#include <ratio>

#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignAvailableProcs.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/debug/safe_compare.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/utility/assign_utils.hpp"

#include "uit/setup/ImplSelect.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/spouts/wrappers/CachingSpoutWrapper.hpp"

#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNodeInput.hpp"
#include "netuit/mesh/MeshNodeOutput.hpp"

using MSG_T = int;
using Spec = uit::ImplSpec<MSG_T, uit::ImplSelect<>, uit::CachingSpoutWrapper>;

#define REPEAT for (size_t rep = 0; rep < std::deca{}.num; ++rep)

#define CSW_IMPL_NAME "CachingSpoutWrapper"

inline decltype(auto) make_dyadic_bundle() {

  netuit::Mesh<Spec> mesh{
    netuit::DyadicTopologyFactory{}(uitsl::get_nprocs()),
    uitsl::AssignIntegrated<uitsl::thread_id_t>{},
    netuit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();
  REQUIRE( bundles.size() == 1 );

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

};

TEST_CASE("Is initial CachingSpoutWrapper Get() result value-intialized? " CSW_IMPL_NAME, "[CachingSpoutWrapper]") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  REQUIRE( input.Get() == MSG_T{} );
  REQUIRE( input.JumpGet() == MSG_T{} );

} }

TEST_CASE("Unmatched gets " CSW_IMPL_NAME, "[CachingSpoutWrapper]") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {
    REQUIRE( input.JumpGet() == MSG_T{} );
  }

  UITSL_Barrier( MPI_COMM_WORLD );

  output.Put(42);
  output.TryFlush();
  while( input.JumpGet() != 42);

  for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {
    REQUIRE( input.JumpGet() == 42 );
  }

} }

TEST_CASE("Unmatched puts " CSW_IMPL_NAME, "[CachingSpoutWrapper]") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) output.TryPut(i);

  REQUIRE( uitsl::safe_leq(input.JumpGet(), 2 * uit::DEFAULT_BUFFER) );

  output.TryFlush();

  REQUIRE( uitsl::safe_leq(input.JumpGet(), 2 * uit::DEFAULT_BUFFER) );

  UITSL_Barrier( MPI_COMM_WORLD ); // todo why

} }

TEST_CASE("Eventual flush-out " CSW_IMPL_NAME, "[CachingSpoutWrapper]") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) output.TryPut(0);

  output.TryFlush();

  while ( !output.TryPut( 1 ) ) {
    const auto res{ input.JumpGet() }; // operational!
    REQUIRE( std::unordered_set{0, 1}.count(res) );
  }

  output.TryFlush();

  while ( input.JumpGet() != 1 ) {
    const auto res{ input.JumpGet() }; // operational!
    REQUIRE( std::unordered_set{0, 1}.count(res) );
  }

  REQUIRE( input.JumpGet() == 1 );

  UITSL_Barrier( MPI_COMM_WORLD ); // todo why

} }

TEST_CASE("Validity " CSW_IMPL_NAME, "[CachingSpoutWrapper]") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  int last{};
  for (MSG_T msg = 0; msg < 10 * std::kilo{}.num; ++msg) {

    output.TryPut(msg);
    output.TryFlush();

    const MSG_T current = input.JumpGet();
    REQUIRE( current >= 0 );
    REQUIRE( current < 10 * std::kilo{}.num );
    REQUIRE( last <= current);

    last = current;

  }

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }
