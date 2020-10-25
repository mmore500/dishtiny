#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/web/fill_colormaps/DummyFillColorMap.hpp"
#include "dish2/web/getters/DummyGetter.hpp"

TEST_CASE("Test FillColorMap") {
  using getter_t = dish2::DummyGetter<size_t>;
  dish2::DummyFillColorMap<getter_t>{ getter_t{} };
}
