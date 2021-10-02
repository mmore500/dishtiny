#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/peripheral/readable_state/ReadableState.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test ReadableState") {

  dish2::ReadableState<Spec>{};

  dish2::ReadableState<Spec>{}.Read(0);

}
