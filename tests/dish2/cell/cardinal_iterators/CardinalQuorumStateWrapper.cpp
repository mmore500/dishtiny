#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/cardinal_iterators/CardinalQuorumStateWrapper.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test CardinalQuorumStateWrapper") {
  dish2::CardinalQuorumStateWrapper<Spec>{};
}
