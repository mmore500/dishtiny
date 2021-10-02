#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/peripheral/readable_state/writable_state/controller_mapped_state/ControllerMappedState.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

TEST_CASE("Test ControllerMappedState") {
  dish2::ControllerMappedState<Spec>{};
}
