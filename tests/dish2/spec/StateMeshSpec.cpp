#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/Spec.hpp"
#include "dish2/spec/StateMeshSpec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test StateMeshSpec") {
  dish2::StateMeshSpec<Spec>{};
}
