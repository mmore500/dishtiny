#include <sstream>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/compare_streams.hpp"

TEST_CASE("compare_streams same", "[nproc:1]") {

  std::stringstream first;
  std::stringstream second;

  first << "abc123";
  second << "abc123";

  REQUIRE( uitsl::compare_streams( first, second ) );

}

TEST_CASE("compare_streams different content", "[nproc:1]") {

  std::stringstream first;
  std::stringstream second;

  first << "abc123";
  second << "xyz789";

  REQUIRE( !uitsl::compare_streams( first, second ) );

}

TEST_CASE("compare_streams different lengths", "[nproc:1]") {

  std::stringstream first;
  std::stringstream second;

  first << "abc123";
  second << "x9";

  REQUIRE( !uitsl::compare_streams( first, second ) );

}
