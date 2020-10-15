#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/peripheral/readable_state/ReadableState.hpp"

TEST_CASE("Test ReadableState") {

  dish2::ReadableState{};

  dish2::ReadableState{}.Read(0);

}
