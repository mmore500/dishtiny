#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/GenomeMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test GenomeMeshSpec") {
  dish2::GenomeMeshSpec<Spec>{};
}
