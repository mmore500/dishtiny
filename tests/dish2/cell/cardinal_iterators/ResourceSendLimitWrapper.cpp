#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/cardinal_iterators/ResourceSendLimitWrapper.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test ResourceSendLimitWrapper") {
  dish2::ResourceSendLimitWrapper<Spec>{};
}
