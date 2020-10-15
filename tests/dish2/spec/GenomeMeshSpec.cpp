#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/GenomeMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"

TEST_CASE("Test GenomeMeshSpec") {
  dish2::GenomeMeshSpec<dish2::Spec>{};
}
