#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/fixtures/Source.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test Source") {

  // TODO flesh out stub test
  uit::Source<uit::ImplSpec<char>> source;
  source.get<0>();

  [[maybe_unused]] auto& [outlet] = source;

}
