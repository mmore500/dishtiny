#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/cell/should_runs/intra_message_launching_should_run.hpp"

TEST_CASE("Test intra_message_launching_should_run") {
  dish2::intra_message_launching_should_run(0, false);
}
