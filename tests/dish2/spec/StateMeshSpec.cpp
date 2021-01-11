#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/StateMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"

TEST_CASE("Test StateMeshSpec") {
  dish2::StateMeshSpec<dish2::Spec>{};
}
