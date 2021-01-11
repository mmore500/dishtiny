#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/QuorumMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"

TEST_CASE("Test QuorumMeshSpec") {
  dish2::QuorumMeshSpec<dish2::Spec>{};
}
