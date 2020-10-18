#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/should_runs/birth_setup_should_run.hpp"

TEST_CASE("Test birth_setup_should_run") {
  dish2::birth_setup_should_run(0, false);
}
