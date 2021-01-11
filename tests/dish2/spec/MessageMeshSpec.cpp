#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/MessageMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"

TEST_CASE("Test MessageMeshSpec") {
  dish2::MessageMeshSpec<dish2::Spec>{};
}
