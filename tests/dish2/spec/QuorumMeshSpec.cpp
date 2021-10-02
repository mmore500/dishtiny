#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/QuorumMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test QuorumMeshSpec") {
  dish2::QuorumMeshSpec<Spec>{};
}
