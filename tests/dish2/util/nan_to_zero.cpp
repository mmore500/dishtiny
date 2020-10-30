#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/util/nan_to_zero.hpp"

TEST_CASE("Test respect non-nan values, float") {
  REQUIRE( dish2::nan_to_zero( 0.0f ) == 0.0f );
  REQUIRE( dish2::nan_to_zero( 1.0f ) == 1.0f );
  REQUIRE( dish2::nan_to_zero( -1.0f ) == -1.0f );
  REQUIRE( dish2::nan_to_zero( 1.0f / 0.0f ) ==  1.0f / 0.0f );
  REQUIRE( dish2::nan_to_zero( -1.0f / 0.0f ) ==  -1.0f / 0.0f );
}

TEST_CASE("Test sanitize nan values, float") {
  REQUIRE( dish2::nan_to_zero( 0.0f/0.0f ) == 0.0f );
  REQUIRE( dish2::nan_to_zero( -0.0f/0.0f ) == 0.0f );
}

TEST_CASE("Test respect non-nan values, double") {
  REQUIRE( dish2::nan_to_zero( 0.0 ) == 0.0 );
  REQUIRE( dish2::nan_to_zero( 1.0 ) == 1.0 );
  REQUIRE( dish2::nan_to_zero( -1.0 ) == -1.0 );
  REQUIRE( dish2::nan_to_zero( 1.0 / 0.0 ) ==  1.0 / 0.0 );
  REQUIRE( dish2::nan_to_zero( -1.0 / 0.0 ) ==  -1.0 / 0.0 );
}

TEST_CASE("Test sanitize nan values, double") {
  REQUIRE( dish2::nan_to_zero( 0.0/0.0 ) == 0.0 );
  REQUIRE( dish2::nan_to_zero( -0.0/0.0 ) == 0.0 );
}
