#include <ratio>
#include <thread>
#include <type_traits>
#include <unordered_set>

#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/optional.hpp"

#include "uitsl/concurrent/Gatherer.hpp"
#include "uitsl/debug/benchmark_utils.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/debug/safe_compare.hpp"
#include "uitsl/math/math_utils.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/nonce/CircularIndex.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/polyfill/barrier.hpp"
#include "uitsl/utility/assign_utils.hpp"

#include "uit/fixtures/Source.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/arrange/ProConTopologyFactory.hpp"
#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"

using MSG_T = int;
using Spec = uit::ImplSpec<MSG_T, ImplSel>;

#define REPEAT for (size_t rep = 0; rep < std::deca::num ; ++rep)

#define THREADED_BEGIN uitsl::ThreadTeam team; for (uitsl::thread_id_t thread_id = 0; thread_id < TestType::value; ++thread_id) { team.Add([&, thread_id](){

#define THREADED_END }); } team.Join();

#define TD_IMPL_NAME IMPL_NAME " ThreadProcDuct"

using two_thread = std::integral_constant<int, 2>;
using three_thread = std::integral_constant<int, 3>;

// this test is required for preventing overlow
// of mesh IDs when running tests from many files back to back
TEST_CASE("Reset MeshIDCounter" TD_IMPL_NAME, "[ThreadDuct][nproc:1]") {
  netuit::internal::MeshIDCounter::Reset();
  REQUIRE(netuit::internal::MeshIDCounter::Get() == 0);
}

TEMPLATE_TEST_CASE("Is initial ThreadDuct Get() result value-intialized? " TD_IMPL_NAME, "[ThreadDuct][nproc:1]", two_thread, three_thread) {  REPEAT {

  auto [outlet] = uit::Source<Spec>{
    std::in_place_type_t<Spec::ThreadDuct>{}
  };

  REQUIRE( outlet.JumpGet() == MSG_T{} );

} }

TEMPLATE_TEST_CASE("Unmatched gets " TD_IMPL_NAME, "[ThreadDuct][nproc:1]", two_thread, three_thread) { REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::DyadicTopologyFactory{}(TestType::value),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {
      REQUIRE( input.JumpGet() == MSG_T{} );
    }

  } THREADED_END

} }

TEMPLATE_TEST_CASE("Unmatched puts " TD_IMPL_NAME, "[ThreadDuct][nproc:1]", two_thread, three_thread) { REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::DyadicTopologyFactory{}(TestType::value),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) output.TryPut(i);

    REQUIRE( uitsl::safe_leq(
        input.JumpGet(),
        (2 * uit::DEFAULT_BUFFER) * (2 * uit::DEFAULT_BUFFER)
    ) );

  } THREADED_END

} }

/*
int main( int argc, char* argv[] ) {

  Catch::Session session; // There must be exactly one instance

  // Build a new parser on top of Catch's
  using namespace Catch::clara;
  session.cli(
    // Get Catch's composite command line parser
    session.cli()
    // bind variable to a new option, with a hint string
    | Opt( num_threads, "num_threads" )
      // the option names it will respond to
      ["-t"]["--num_threads"]
      // description string for the help output
      ("how many threds to test with?")
  );

  // Let Catch (using Clara) parse the command line
  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 ) return returnCode;

  barrier.emplace( uitsl::safe_cast<std::ptrdiff_t>(num_threads) );

  return session.run();
}
*/
