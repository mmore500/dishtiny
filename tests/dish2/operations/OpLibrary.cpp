#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/operations/OpLibrary.hpp"
#include "dish2/spec/Spec.hpp"

TEST_CASE("Test OpLibrary") {
  dish2::OpLibrary<dish2::Spec>{};
}
