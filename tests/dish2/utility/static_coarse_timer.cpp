#include <chrono>
#include <thread>

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/utility/static_coarse_timer.hpp"

TEST_CASE("static_coarse_timer") {

  using namespace std::chrono_literals;

  REQUIRE( dish2::static_coarse_timer.GetElapsed() < 3s );

  std::this_thread::sleep_for( 5s );

  REQUIRE( dish2::static_coarse_timer.GetElapsed() > 2s );

}
