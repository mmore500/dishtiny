#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/web/Canvas.h"

#include "dish2/web/util/CanvasMathHelper.hpp"

TEST_CASE("Test CanvasMathHelper") {
  emp::web::Canvas canvas(500, 500);
  dish2::CanvasMathHelper{ canvas };
}
