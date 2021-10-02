#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/cardinal_iterators/StateNodeInputWrapper.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test StateNodeInputWrapper") {
  dish2::StateNodeInputWrapper<Spec>{};
}
