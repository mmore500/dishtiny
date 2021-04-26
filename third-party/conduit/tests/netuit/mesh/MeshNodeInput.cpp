#include "Catch/single_include/catch2/catch.hpp"

#include "uit/fixtures/Source.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/mesh/MeshNodeInput.hpp"

TEST_CASE("Test MeshNodeInput", "[nproc:1]") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::Source<Spec> source;

  netuit::MeshNodeInput<Spec>{
    source.GetOutlet(),
    0
  };

}
