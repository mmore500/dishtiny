#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/web/Canvas.h"

#include "dish2/config/cfg.hpp"
#include "dish2/web/util/CanvasMathHelper.hpp"

TEST_CASE("Test CanvasMathHelper") {

  dish2::cfg.Set( "N_CELLS", "100" );

  emp::web::Canvas canvas(500, 500);
  dish2::CanvasMathHelper helper{ canvas };

  REQUIRE( helper.GetGridWidth() == 10 );
  REQUIRE( helper.GetGridHeight() == 10 );

  REQUIRE( helper.GetCellWidthPx() == 50 );
  REQUIRE( helper.GetCellHeightPx() == 50 );

  REQUIRE( helper.GetCanvasX(0) == 0 );
  REQUIRE( helper.GetCanvasX(1) == 50 );

  REQUIRE( helper.GetCanvasY(0) == 0 );
  REQUIRE( helper.GetCanvasY(1) == 50 );

}
