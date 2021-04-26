#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/fixtures/Sink.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test Sink") {

  // TODO flesh out stub test
  uit::Sink<uit::ImplSpec<char>> sink;
  sink.get<0>();

  [[maybe_unused]] auto& [inlet] = sink;

}
