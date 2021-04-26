#include <ratio>
#include <thread>
#include <type_traits>

#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignAvailableProcs.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/debug/safe_compare.hpp"
#include "uitsl/distributed/RdmaWindowManager.hpp"
#include "uitsl/math/math_utils.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/nonce/CircularIndex.hpp"
#include "uitsl/utility/assign_utils.hpp"

#include "uit/fixtures/Sink.hpp"
#include "uit/fixtures/Source.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/arrange/ProConTopologyFactory.hpp"
#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNodeInput.hpp"
#include "netuit/mesh/MeshNodeOutput.hpp"

using MSG_T = int;
using Spec = uit::ImplSpec<MSG_T, ImplSel>;

#define REPEAT for (size_t rep = 0; rep < std::deca::num; ++rep)

#define PD_IMPL_NAME IMPL_NAME "ProcDuct"
#ifndef TAGS
#define TAGS ""
#endif

// this test is required for preventing overlow
// of mesh IDs when running tests from many files back to back
TEST_CASE("Reset MeshIDCounter" PD_IMPL_NAME, "[ProcDuct][nproc:1]") {
  netuit::internal::MeshIDCounter::Reset();
  REQUIRE(netuit::internal::MeshIDCounter::Get() == 0);
}


template <typename T>
decltype(auto) make_dyadic_pd_bundle() {

  netuit::Mesh<T> mesh{
    netuit::DyadicTopologyFactory{}(uitsl::get_nprocs()),
    uitsl::AssignIntegrated<uitsl::thread_id_t>{},
    netuit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();
  REQUIRE( bundles.size() == 1 );

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

};

template <typename T>
decltype(auto) make_producer_consumer_pd_bundle() {

  netuit::Mesh<T> mesh{
    netuit::ProConTopologyFactory{}(uitsl::get_nprocs()),
    uitsl::AssignIntegrated<uitsl::thread_id_t>{},
    netuit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh(0);

  REQUIRE( bundles.size() == 1 );
  REQUIRE(
    (bundles[0].GetInputOrNullopt(0) || bundles[0].GetOutputOrNullopt(0))
  );

  return std::tuple{
    bundles[0].GetInputOrNullopt(0),
    bundles[0].GetOutputOrNullopt(0)
  };

};

template <typename T>
decltype(auto) make_ring_pd_bundle() {
  netuit::Mesh<T> mesh{
    netuit::RingTopologyFactory{}(uitsl::get_nprocs()),
    uitsl::AssignIntegrated<uitsl::thread_id_t>{},
    netuit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 1);

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

}

// p_1 -> p_2 -> ... -> p_n -> p_1 -> p_2 -> ... -> p_n -> p_1
template <typename T>
decltype(auto) make_coiled_pd_bundle() {
  netuit::Mesh<T> mesh{
    netuit::RingTopologyFactory{}(uitsl::get_nprocs() * 2),
    uitsl::AssignIntegrated<uitsl::thread_id_t>{},
    uitsl::AssignRoundRobin<uitsl::proc_id_t>{
      uitsl::safe_cast<size_t>( uitsl::get_nprocs() )
    }
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 2);

  typename std::decay<decltype(bundles[0].GetInputs())>::type inputs;
  for (const auto& bundle : bundles) {
    inputs.insert(
      std::end(inputs),
      std::begin(bundle.GetInputs()),
      std::end(bundle.GetInputs())
    );
  }

  typename std::decay<decltype(bundles[0].GetOutputs())>::type outputs;
  for (const auto& bundle : bundles) {
    outputs.insert(
      std::end(outputs),
      std::begin(bundle.GetOutputs()),
      std::end(bundle.GetOutputs())
    );
  }

  return std::tuple{ inputs, outputs };

}

// TODO why doesn't this work with openmpi with RingRdmaDuct?
// TEST_CASE("Is initial RingRdmaDuct Get() result value-intialized?" PD_IMPL_NAME) { REPEAT {
//
//   std::shared_ptr<Spec::ProcBackEnd> backend{
//     std::make_shared<Spec::ProcBackEnd>()
//   };
//
//   auto [outlet] = uit::Source<Spec>{
//     std::in_place_type_t<Spec::ProcOutletDuct>{},
//     uit::InterProcAddress{
//       uitsl::get_rank(),
//       uitsl::safe_cast<int>(
//         uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), 1)
//       )
//     },
//     backend
//   };
//
//   // corresponding sink
//   uit::Sink<Spec>{
//     std::in_place_type_t<Spec::ProcOutletDuct>{},
//     uit::InterProcAddress{
//       uitsl::get_rank(),
//       uitsl::safe_cast<int>(
//         uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
//       )
//     },
//     backend
//   };
//
//   backend->Initialize();
//
//   REQUIRE( outlet.Get() == MSG_T{} );
//   REQUIRE( outlet.JumpGet() == MSG_T{} );
//
// } }

TEST_CASE("Is initial ProcDuct Get() result value-intialized?" PD_IMPL_NAME, "[ProcDuct]" TAGS) { REPEAT {

  auto [input, output] = make_ring_pd_bundle<Spec>();

  REQUIRE( input.Get() == MSG_T{} );
  REQUIRE( input.JumpGet() == MSG_T{} );

} }

TEST_CASE("Unmatched gets" PD_IMPL_NAME, "[ProcDuct]" TAGS) { REPEAT {

  auto [input, output] = make_dyadic_pd_bundle<Spec>();

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

TEST_CASE("Unmatched puts" PD_IMPL_NAME, "[ProcDuct]" TAGS) { REPEAT {

  auto [input, output] = make_dyadic_pd_bundle<Spec>();

  for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) output.TryPut(i);

  REQUIRE( uitsl::safe_leq(input.JumpGet(), 2 * uit::DEFAULT_BUFFER) );

  output.TryFlush();

  REQUIRE( uitsl::safe_leq(input.JumpGet(), 2 * uit::DEFAULT_BUFFER) );

  UITSL_Barrier( MPI_COMM_WORLD ); // todo why

} }

TEST_CASE("Eventual flush-out" PD_IMPL_NAME, "[ProcDuct]" TAGS) { REPEAT {

  auto [input, output] = make_dyadic_pd_bundle<Spec>();

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

TEST_CASE("Validity" PD_IMPL_NAME, "[ProcDuct]" TAGS) { REPEAT {

  auto [input, output] = make_dyadic_pd_bundle<Spec>();

  int last{};
  for (MSG_T msg = 0; msg < 10 * std::kilo::num; ++msg) {

    output.TryPut(msg);
    output.TryFlush();

    const MSG_T current = input.JumpGet();
    REQUIRE( current >= 0 );
    REQUIRE( current < 10 * std::kilo::num );
    REQUIRE( last <= current);

    last = current;

  }

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Multi-bridge Validity" PD_IMPL_NAME, "[ProcDuct]" TAGS) { REPEAT {

  auto [inputs, outputs] = make_coiled_pd_bundle<Spec>();

  std::unordered_map<size_t, int> last_map{};
  for (MSG_T msg = 0; msg < 10 * std::kilo::num; ++msg) {

    for (auto& output : outputs) {
      output.TryPut(msg);
      output.TryFlush();
    }

    for (size_t i = 0; i < inputs.size(); ++i) {
      const MSG_T current = inputs[i].JumpGet();
      REQUIRE( current >= 0 );
      REQUIRE( current < 10 * std::kilo::num );
      REQUIRE( last_map[i] <= current);

      last_map[i] = current;
    }

  }

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }
