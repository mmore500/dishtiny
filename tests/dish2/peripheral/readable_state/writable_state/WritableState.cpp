#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/peripheral/readable_state/writable_state/WritableState.hpp"

TEST_CASE("Test WritableState") {
  dish2::WritableState{};
  dish2::WritableState{}.Write( 0, 27.0 );
}
