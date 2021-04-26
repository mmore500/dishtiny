#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/chrono/RawCycleClock.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"

using mytimer_t = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;
using bar_t = uitsl::ProgressBar<mytimer_t>;

TEST_CASE("Test RawCycleClock", "[nproc:1]")
{

  uitsl::RawCycleClock{};

  const auto begin = uitsl::RawCycleClock::now();

  for ( [[maybe_unused]] auto it : bar_t{ std::chrono::seconds{3} }) {
    REQUIRE( uitsl::RawCycleClock::now() >= begin );
  }

  REQUIRE( uitsl::RawCycleClock::now() > begin );

}
