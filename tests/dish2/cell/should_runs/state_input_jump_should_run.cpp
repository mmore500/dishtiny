#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/should_runs/state_input_jump_should_run.hpp"

TEST_CASE("Test state_input_jump_should_run") {
  dish2::state_input_jump_should_run(0, false);
}
