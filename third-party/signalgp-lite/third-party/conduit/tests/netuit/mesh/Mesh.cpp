#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/mpi_guard.hpp"

#include "uit/setup/ImplSpec.hpp"

#include "netuit/arrange/ProConTopologyFactory.hpp"
#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/arrange/ToroidalGridTopologyFactory.hpp"
#include "netuit/arrange/ToroidalTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"

TEST_CASE("Test Mesh", "[nproc:1]") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  netuit::Mesh<Spec> mesh{netuit::RingTopologyFactory{}(100)};

  REQUIRE( mesh.GetNodeCount() == 100 );
  REQUIRE( mesh.GetEdgeCount() == 100 );
  REQUIRE( mesh.GetSubmesh().size() == 100 );

}

TEST_CASE("Test with ProConTopologyFactory", "[nproc:1]") {

  using Spec = uit::ImplSpec<char>;

  netuit::Mesh<Spec> mesh{netuit::ProConTopologyFactory{}(100)};

  REQUIRE( mesh.GetNodeCount() == 100 );
  REQUIRE( mesh.GetEdgeCount() == 50 );
  REQUIRE( mesh.GetSubmesh().size() == 100 );

}

// TODO add tests with more TopologyFactories
// TODO add tests with no-connection nodes

TEST_CASE("Test ToroidalGridMesh reciporical", "[nproc:1]") {

  using Spec = uit::ImplSpec<size_t>;

  netuit::Mesh<Spec> mesh{
    netuit::ToroidalGridTopologyFactory{}(100)
  };

  REQUIRE( mesh.GetNodeCount() == 100 );
  REQUIRE( mesh.GetEdgeCount() == 400 );

  auto submesh = mesh.GetSubmesh();

  size_t counter{};
  for (auto& node : submesh) {
    for (auto& output : node.GetOutputs()) {
      output.Put(counter++);
    }
  }

  for (auto& node : submesh) {
    REQUIRE( node.GetNumInputs() == node.GetNumOutputs() );
    for (size_t i{}; i < node.GetNumInputs(); ++i) {
      node.GetOutput( i ).Put( node.GetInput( i ).GetNext() );
    }
  }

  counter = 0;
  for (auto& node : submesh) {
    for (auto& input : node.GetInputs()) {
      REQUIRE( input.GetNext() == counter++ );
    }
  }


}

TEST_CASE("Test ToroidalMesh reciporical", "[nproc:1]") {

  using Spec = uit::ImplSpec<size_t>;

  netuit::Mesh<Spec> mesh{
    netuit::ToroidalTopologyFactory{}( {10, 10, 10} )
  };

  REQUIRE( mesh.GetNodeCount() == 1000 );
  REQUIRE( mesh.GetEdgeCount() == 6000 );

  auto submesh = mesh.GetSubmesh();

  size_t counter{};
  for (auto& node : submesh) {
    for (auto& output : node.GetOutputs()) {
      output.Put(counter++);
    }
  }

  for (auto& node : submesh) {
    REQUIRE( node.GetNumInputs() == node.GetNumOutputs() );
    for (size_t i{}; i < node.GetNumInputs(); ++i) {
      node.GetOutput( i ).Put( node.GetInput( i ).GetNext() );
    }
  }

  counter = 0;
  for (auto& node : submesh) {
    for (auto& input : node.GetInputs()) {
      REQUIRE( input.GetNext() == counter++ );
    }
  }


}

TEST_CASE("Test ToroidalMesh reciporical not square", "[nproc:1]") {

  using Spec = uit::ImplSpec<size_t>;

  netuit::Mesh<Spec> mesh{
    netuit::ToroidalTopologyFactory{}( {10, 20, 5} )
  };

  REQUIRE( mesh.GetNodeCount() == 1000 );
  REQUIRE( mesh.GetEdgeCount() == 6000 );

  auto submesh = mesh.GetSubmesh();

  size_t counter{};
  for (auto& node : submesh) {
    for (auto& output : node.GetOutputs()) {
      output.Put(counter++);
    }
  }

  for (auto& node : submesh) {
    REQUIRE( node.GetNumInputs() == node.GetNumOutputs() );
    for (size_t i{}; i < node.GetNumInputs(); ++i) {
      node.GetOutput( i ).Put( node.GetInput( i ).GetNext() );
    }
  }

  counter = 0;
  for (auto& node : submesh) {
    for (auto& input : node.GetInputs()) {
      REQUIRE( input.GetNext() == counter++ );
    }
  }


}
