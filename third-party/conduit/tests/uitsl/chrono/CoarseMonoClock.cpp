#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/chrono/CoarseMonoClock.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"

using mytimer_t = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;
using bar_t = uitsl::ProgressBar<mytimer_t>;

TEST_CASE("Test CoarseMonoClock", "[nproc:1]")
{

  uitsl::CoarseMonoClock{};

  const auto begin = uitsl::CoarseMonoClock::now();

  for ( [[maybe_unused]] auto it : bar_t{ std::chrono::seconds{3} }) {
    REQUIRE( uitsl::CoarseMonoClock::now() >= begin );
  }

  REQUIRE( uitsl::CoarseMonoClock::now() - begin > std::chrono::seconds{1} );
  REQUIRE( uitsl::CoarseMonoClock::now() - begin < std::chrono::seconds{5} );

}
