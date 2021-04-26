#include <limits>
#include <unordered_set>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/math/ratio_to_double.hpp"

TEST_CASE("Test ratio_to_double", "[nproc:1]") {

  REQUIRE( uitsl::ratio_to_double<std::deca>() == 10.0 );
  REQUIRE( uitsl::ratio_to_double<std::ratio<1>>() == 1.0 );
  REQUIRE( uitsl::ratio_to_double<std::deci>() == 0.1 );

}

TEST_CASE("Test as_double", "[nproc:1]") {

  REQUIRE(
    uitsl::as_double<std::deca>() == uitsl::ratio_to_double<std::deca>()
  );

  REQUIRE(
    uitsl::as_double<std::ratio<1>>() == uitsl::ratio_to_double<std::ratio<1>>()
  );

  REQUIRE(
    uitsl::as_double<std::deci>() == uitsl::ratio_to_double<std::deci>()
  );

}
