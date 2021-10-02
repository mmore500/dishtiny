#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/viz/getters/DummyGetter.hpp"

TEST_CASE("Test DummyGetter") {
  dish2::DummyGetter<size_t>{};
}
