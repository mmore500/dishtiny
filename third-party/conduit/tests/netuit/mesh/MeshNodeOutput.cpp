#include "Catch/single_include/catch2/catch.hpp"

#include "uit/fixtures/Sink.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/mesh/MeshNodeOutput.hpp"

TEST_CASE("Test MeshNodeOutput", "[nproc:1]") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::Sink<Spec> sink;

  // TODO flesh out stub test
  netuit::MeshNodeOutput<Spec>{
    sink.GetInlet(),
    0
  };

}
