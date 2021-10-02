#include <set>

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/config/calc_rng_preseed.hpp"
#include "dish2/config/cfg.hpp"


TEST_CASE("calc_rng_preseed determinstic") {

  REQUIRE( dish2::calc_rng_preseed() == dish2::calc_rng_preseed() );

}

TEST_CASE("calc_rng_preseed changed by STINT") {

  const auto first = dish2::calc_rng_preseed();

  dish2::cfg.Set("STINT", "0");
  const auto second = dish2::calc_rng_preseed();
  REQUIRE( first != second );

  dish2::cfg.Set("STINT", "1");
  const auto third = dish2::calc_rng_preseed();
  REQUIRE( first != third);

  REQUIRE( second != third );

  dish2::cfg.Reset();

}

TEST_CASE("calc_rng_preseed changed by SERIES") {

  const auto first = dish2::calc_rng_preseed();

  dish2::cfg.Set("SERIES", "0");
  const auto second = dish2::calc_rng_preseed();
  REQUIRE( first != second );

  dish2::cfg.Set("SERIES", "1");
  const auto third = dish2::calc_rng_preseed();
  REQUIRE( first != third);

  REQUIRE( second != third );

  dish2::cfg.Reset();

}

TEST_CASE("calc_rng_preseed changed by REPLICATE") {

  const auto first = dish2::calc_rng_preseed();

  dish2::cfg.Set("REPLICATE", "0");
  const auto second = dish2::calc_rng_preseed();
  REQUIRE( first != second );

  dish2::cfg.Set("REPLICATE", "1");
  const auto third = dish2::calc_rng_preseed();
  REQUIRE( first != third);

  REQUIRE( second != third );

  dish2::cfg.Reset();

}

TEST_CASE("calc_rng_preseed changed by combinations") {

  const auto first = dish2::calc_rng_preseed();

  dish2::cfg.Set("STINT", "0");
  const auto second = dish2::calc_rng_preseed();
  REQUIRE( first != second );

  dish2::cfg.Set("SERIES", "0");
  const auto third = dish2::calc_rng_preseed();
  REQUIRE( first != third );

  dish2::cfg.Set("REPLICATE", "0");
  const auto fourth = dish2::calc_rng_preseed();
  REQUIRE( first != fourth );

  REQUIRE( std::set{ first, second, third, fourth }.size() == 4 );

  dish2::cfg.Reset();

}
