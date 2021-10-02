#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/cardinal_iterators/ResourceReserveRequestWrapper.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test ResourceReserveRequestWrapper") {
  dish2::ResourceReserveRequestWrapper<Spec>{};
}
