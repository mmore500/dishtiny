#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/Spec.hpp"
#include "dish2/web/fill_colormaps/ResourceStockpileColorMap.hpp"

TEST_CASE("Test ResourceStockpileColorMap") {
  dish2::ResourceStockpileColorMap{};
}
