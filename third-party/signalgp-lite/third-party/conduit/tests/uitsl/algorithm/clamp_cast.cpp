#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/algorithm/clamp_cast.hpp"

TEST_CASE("Test clamp_cast no overflow", "[nproc:1]") {

  REQUIRE( uitsl::clamp_cast<int>( 0.1 ) == 0 );
  REQUIRE( uitsl::clamp_cast<int>( -10.0 ) == -10 );
  REQUIRE( uitsl::clamp_cast<int>( 100.0f ) == 100 );

}

TEST_CASE("Test clamp_cast with finite overflow", "[nproc:1]") {

  REQUIRE( uitsl::clamp_cast<int>(
    static_cast<double>( std::numeric_limits<int>::max() ) * 2
  ) == std::numeric_limits<int>::max() );
  REQUIRE( uitsl::clamp_cast<char>(
    static_cast<double>( std::numeric_limits<char>::min() ) * 2
  ) == std::numeric_limits<char>::min() );

}

TEST_CASE("Test clamp_cast with infinite overflow", "[nproc:1]") {

  REQUIRE( uitsl::clamp_cast<int>(
    std::numeric_limits<double>::infinity()
  ) == std::numeric_limits<int>::max() );
  REQUIRE( uitsl::clamp_cast<int>(
    -std::numeric_limits<double>::infinity()
  ) == std::numeric_limits<int>::lowest() );
  REQUIRE( uitsl::clamp_cast<size_t>(
    -std::numeric_limits<double>::infinity()
  ) == std::numeric_limits<size_t>::lowest() );

  REQUIRE( uitsl::clamp_cast<double>(
    std::numeric_limits<double>::infinity()
  ) == std::numeric_limits<double>::max() );
  REQUIRE( std::isfinite( uitsl::clamp_cast<double>(
    std::numeric_limits<double>::infinity()
  ) ) );

  REQUIRE( uitsl::clamp_cast<double>(
    -std::numeric_limits<double>::infinity()
  ) == std::numeric_limits<double>::lowest() );
  REQUIRE( std::isfinite( uitsl::clamp_cast<double>(
    -std::numeric_limits<double>::infinity()
  ) ) );

}

TEST_CASE("Test clamp_cast with nan", "[nproc:1]") {

  REQUIRE( uitsl::clamp_cast<int>(
    std::numeric_limits<double>::quiet_NaN()
  ) == 0);
  REQUIRE( uitsl::clamp_cast<size_t>(
    std::numeric_limits<double>::quiet_NaN()
  ) == 0 );

}
