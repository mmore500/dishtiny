#include <limits>

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/viz/fill_colormaps/StateRegisterColorMap.hpp"

TEST_CASE("Test StateRegisterColorMap") {
  const dish2::StateRegisterColorMap cmap{};

  REQUIRE( cmap.Paint( 0.0f ) == "white" );
  REQUIRE( cmap.Paint( std::numeric_limits<float>::quiet_NaN() ) == "purple" );

  // opposite signs map to different hues
  REQUIRE( cmap.Paint( 1.0f ) != cmap.Paint( -1.0f ) );

  // greater magnitude maps to a different (more saturated) color
  REQUIRE( cmap.Paint( 1.0f ) != cmap.Paint( 10.0f ) );
  REQUIRE( cmap.Paint( -1.0f ) != cmap.Paint( -10.0f ) );

  // small nonzero values are distinguishable from zero
  REQUIRE( cmap.Paint( 0.001f ) != cmap.Paint( 0.0f ) );
  REQUIRE( cmap.Paint( -0.001f ) != cmap.Paint( 0.0f ) );

  // infinities don't blow up
  cmap.Paint( std::numeric_limits<float>::infinity() );
  cmap.Paint( -std::numeric_limits<float>::infinity() );
}
