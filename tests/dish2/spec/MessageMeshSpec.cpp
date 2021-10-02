#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/MessageMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test MessageMeshSpec") {
  dish2::MessageMeshSpec<Spec>{};
}
