#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/cardinal_iterators/ResourceNodeOutputWrapper.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test ResourceNodeOutputWrapper") {
  dish2::ResourceNodeOutputWrapper<Spec>{};
}
