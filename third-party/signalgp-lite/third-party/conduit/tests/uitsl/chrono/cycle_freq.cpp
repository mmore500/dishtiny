#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/chrono/cycle_freq.hpp"
#include "uitsl/math/ratio_to_double.hpp"

TEST_CASE("Test measure_cycle_freq", "[nproc:1]") {

  REQUIRE(
    uitsl::measure_cycle_freq() < uitsl::ratio_to_double<std::kilo>()
  );
  REQUIRE(
    uitsl::measure_cycle_freq() > uitsl::ratio_to_double<std::milli>()
  );

}

TEST_CASE("Test estimate_cycle_freq", "[nproc:1]") {

  REQUIRE(
    uitsl::measure_cycle_freq() / uitsl::estimate_cycle_freq()
    > uitsl::ratio_to_double<std::deci>()
  );
  REQUIRE(
    uitsl::measure_cycle_freq() / uitsl::estimate_cycle_freq()
    < uitsl::ratio_to_double<std::deca>()
  );

  REQUIRE(
    uitsl::measure_cycle_freq() / uitsl::estimate_cycle_freq()
    > uitsl::ratio_to_double<std::deci>()
  );
  REQUIRE(
    uitsl::measure_cycle_freq() / uitsl::estimate_cycle_freq()
    < uitsl::ratio_to_double<std::deca>()
  );

}

TEST_CASE("Test get_cycle_freq", "[nproc:1]") {

  REQUIRE( uitsl::get_cycle_freq() == uitsl::get_cycle_freq() );

  REQUIRE(
    uitsl::get_cycle_freq() / uitsl::estimate_cycle_freq()
    > uitsl::ratio_to_double<std::deci>()
  );
  REQUIRE(
    uitsl::get_cycle_freq() / uitsl::estimate_cycle_freq()
    < uitsl::ratio_to_double<std::deca>()
  );

}
