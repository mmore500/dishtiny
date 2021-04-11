#define CATCH_CONFIG_MAIN

#include <chrono>
#include <iostream>
#include <thread>

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"
#include "conduit/include/uitsl/countdown/Timer.hpp"

const uitsl::MpiGuard guard;

#include "dish2/utility/try_with_timeout.hpp"

TEST_CASE("try_with_timeout completes") {

  using namespace std::chrono_literals;

  REQUIRE(
    dish2::try_with_timeout( [](){ std::this_thread::sleep_for(1s); }, 3s )
  );

}

TEST_CASE("try_with_timeout hangs") {

  using namespace std::chrono_literals;

  uitsl::Timer timer;

  REQUIRE(
    !dish2::try_with_timeout( [](){ std::this_thread::sleep_for(1min); }, 3s )
  );

  REQUIRE( timer.GetElapsed() < 10s );

}
