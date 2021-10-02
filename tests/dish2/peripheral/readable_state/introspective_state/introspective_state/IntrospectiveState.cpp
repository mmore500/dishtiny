#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/peripheral/readable_state/introspective_state/IntrospectiveState.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test IntrospectiveState") {
  dish2::IntrospectiveState<Spec>{};
}
