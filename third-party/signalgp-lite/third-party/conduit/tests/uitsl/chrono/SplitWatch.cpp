#include <algorithm>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/chrono/chrono_utils.hpp"
#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/chrono/CycleClock.hpp"
#include "uitsl/chrono/SplitWatch.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"

using mytimer_t = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;
using bar_t = uitsl::ProgressBar<mytimer_t>;

TEST_CASE("Test SplitWatch", "[nproc:1]")
{

  uitsl::SplitWatch<uitsl::CycleClock> watch;

  REQUIRE( watch.TakeSplit() == uitsl::zero_duration );

  for ( [[maybe_unused]] auto it : bar_t{ std::chrono::seconds{1} }) {
    REQUIRE( watch.TakeSplit() >= uitsl::zero_duration );
  }

  std::this_thread::sleep_for( std::chrono::seconds{3} );

  const auto split = watch.TakeSplit();

  REQUIRE( split > std::chrono::seconds{ 1 } );
  REQUIRE( split < std::chrono::seconds{ 5 } );

}
