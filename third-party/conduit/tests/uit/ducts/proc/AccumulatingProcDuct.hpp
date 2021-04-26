#include <ratio>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignAvailableProcs.hpp"
#include "uitsl/distributed/IbarrierRequest.hpp"

#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/arrange/ProConTopologyFactory.hpp"
#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNodeInput.hpp"
#include "netuit/mesh/MeshNodeOutput.hpp"

#define REPEAT for (size_t rep = 0; rep < std::deca::num; ++rep)

#define APD_IMPL_NAME IMPL_NAME "AccumulatingProcDuct"
#ifndef TAGS
#define TAGS ""
#endif


using Spec = uit::ImplSpec<MSG_T, ImplSel>;

template <typename T>
decltype(auto) make_dyadic_apd_bundle() {

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
decltype(auto) make_ring_apd_bundle() {
  netuit::Mesh<T> mesh{
    netuit::RingTopologyFactory{}( uitsl::get_nprocs() ),
    uitsl::AssignIntegrated<uitsl::thread_id_t>{},
    netuit::AssignAvailableProcs{}
  };
  auto bundles = mesh.GetSubmesh();
  REQUIRE( bundles.size() == 1);

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

}
TEST_CASE("Is initial AccumulatingProcDuct Get() result value-intialized? " APD_IMPL_NAME, "[AccumulatingProcDuct]" TAGS) { REPEAT {

  auto [input, output] = make_ring_apd_bundle<Spec>();

  REQUIRE( input.Get() == MSG_T{} );
  REQUIRE( input.JumpGet() == MSG_T{} );

} }

TEST_CASE("Unmatched gets " APD_IMPL_NAME, "[AccumulatingProcDuct]" TAGS) { REPEAT {

  // TODO why does rdma construction hang for dyadic bundle but not ring  ?
  auto [input, output] = make_dyadic_apd_bundle<Spec>();

  for (size_t i = 0; i <= 2 * uit::DEFAULT_BUFFER; ++i) {
    REQUIRE( input.JumpGet() == MSG_T{} );
  }

  UITSL_Barrier( MPI_COMM_WORLD );

  output.Put(42);
  output.TryFlush();

  while( input.JumpGet() != 42);

  REQUIRE( input.Get() == 42 );

  REQUIRE( input.JumpGet() == MSG_T{} );

  UITSL_Barrier( MPI_COMM_WORLD );

} }

TEST_CASE("Unmatched puts " APD_IMPL_NAME, "[AccumulatingProcDuct]" TAGS) { REPEAT {

  // TODO why does rdma construction hang for dyadic bundle but not ring  ?
  auto [input, output] = make_dyadic_apd_bundle<Spec>();

  for (size_t i = 0; i <= 2 * uit::DEFAULT_BUFFER; ++i) output.TryPut(1);

  UITSL_Barrier( MPI_COMM_WORLD ); // todo why

} }

TEST_CASE("Validity " APD_IMPL_NAME, "[AccumulatingProcDuct]" TAGS) { REPEAT {

  // TODO why does rdma construction hang for dyadic bundle but not ring  ?
  auto [input, output] = make_dyadic_apd_bundle<Spec>();

  int sum{};
  // 1/2 n * (n + 1)
  const int expected_sum = (std::kilo::num - 1) * std::kilo::num / 2;
  for (int msg = 0; msg < std::kilo::num; ++msg) {

    output.TryPut(msg);
    output.TryFlush();

    const MSG_T received = input.JumpGet();
    REQUIRE( received <= expected_sum );
    REQUIRE( received >= 0 );
    sum += received;

  }

  while (sum != expected_sum) {
    const MSG_T received = input.JumpGet();
    REQUIRE( received <= expected_sum );
    REQUIRE( received >= 0);
    sum += received;
    output.TryFlush();
  }

  REQUIRE( sum == expected_sum );

  for (size_t i = 0; i < 10 * std::kilo::num; ++i) {
    REQUIRE( input.JumpGet() == MSG_T{} );
  }

  for (uitsl::IbarrierRequest req; !req.IsComplete(); ) output.TryFlush();

} }
