#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/peripheral/readable_state/writable_state/WritableState.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test WritableState") {
  dish2::WritableState<Spec>{};
  dish2::WritableState<Spec>{}.Write( 0, 27.0 );
}
