#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/should_runs/cpu_execution_should_run.hpp"

TEST_CASE("Test cpu_execution_should_run") {
  dish2::cpu_execution_should_run(0, false);
}
