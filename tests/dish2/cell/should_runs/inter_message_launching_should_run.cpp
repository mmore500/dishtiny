#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/should_runs/inter_message_launching_should_run.hpp"

TEST_CASE("Test inter_message_launching_should_run") {
  dish2::inter_message_launching_should_run(0, false);
}
