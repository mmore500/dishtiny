#include "Catch/single_include/catch2/catch.hpp"

#include "uit/fixtures/Conduit.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/mesh/MeshNode.hpp"

TEST_CASE("Test MeshNode", "[nproc:1]") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;
  uit::Conduit<Spec> conduit;

  auto & [inlet, outlet] = conduit;

  // TODO flesh out stub test
  netuit::MeshNode<Spec> node{0};

  node.AddInput(netuit::MeshNodeInput<Spec>{outlet, 0});
  node.AddOutput(netuit::MeshNodeOutput<Spec>{inlet, 0});

}
