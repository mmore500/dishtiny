#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/web/border_colormaps/DummyBorderColorMap.hpp"
#include "dish2/web/getters/DummyGetter.hpp"

TEST_CASE("Test DummyBorderColorMap") {
  using getter_t = dish2::DummyGetter<size_t>;
  dish2::DummyBorderColorMap<getter_t>{ getter_t{} };
}
