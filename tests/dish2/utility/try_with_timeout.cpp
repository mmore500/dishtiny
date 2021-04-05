#define CATCH_CONFIG_MAIN

#include <chrono>
#include <iostream>
#include <thread>

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/utility/try_with_timeout.hpp"

TEST_CASE("try_with_timeout completes") {

  using namespace std::chrono_literals;

  REQUIRE(
    dish2::try_with_timeout( [](){ std::cout << "hello world" << '\n'; }, 3s )
  );

}

TEST_CASE("try_with_timeout hangs") {

  using namespace std::chrono_literals;

  REQUIRE(
    !dish2::try_with_timeout( [](){ std::this_thread::sleep_for(1min); }, 3s )
  );

}
