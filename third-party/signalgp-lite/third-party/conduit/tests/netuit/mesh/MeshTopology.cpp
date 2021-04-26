#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/mpi_guard.hpp"

#include "uit/setup/ImplSpec.hpp"

#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/mesh/MeshTopology.hpp"

TEST_CASE("Test MeshTopology", "[nproc:1]") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  netuit::internal::MeshTopology<Spec> mesh_topology{
    netuit::RingTopologyFactory{}(100)
  };

  REQUIRE( mesh_topology.GetNodeCount() == 100 );
  REQUIRE( mesh_topology.GetEdgeCount() == 100 );

}
