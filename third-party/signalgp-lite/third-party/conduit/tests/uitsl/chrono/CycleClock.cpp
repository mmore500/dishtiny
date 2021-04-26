#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/chrono/CycleClock.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"

using mytimer_t = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;
using bar_t = uitsl::ProgressBar<mytimer_t>;

TEST_CASE("Test CycleClock", "[nproc:1]")
{

  uitsl::CycleClock{};

  const auto begin = uitsl::CycleClock::now();

  for ( [[maybe_unused]] auto it : bar_t{ std::chrono::seconds{3} }) {
    REQUIRE( uitsl::CycleClock::now() >= begin );
  }

  REQUIRE( uitsl::CycleClock::now() - begin > std::chrono::seconds{1} );
  REQUIRE( uitsl::CycleClock::now() - begin < std::chrono::seconds{5} );

}
