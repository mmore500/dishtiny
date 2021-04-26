#include "Catch/single_include/catch2/catch.hpp"

#define __EMSCRIPTEN__
#include "uitsl/polyfill/filesystem.hpp"

TEST_CASE("Test native filesystem", "[nproc:1]") {

  REQUIRE( std::filesystem::path{ "test.txt" }.extension() == ".txt" );

}
