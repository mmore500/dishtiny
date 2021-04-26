#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/fixtures/Conduit.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test Conduit") {

  // TODO flesh out stub test
  uit::Conduit<uit::ImplSpec<char>> conduit;
  conduit.get<0>();
  conduit.get<1>();

  [[maybe_unused]] auto& [inlet, outlet] = conduit;

}
