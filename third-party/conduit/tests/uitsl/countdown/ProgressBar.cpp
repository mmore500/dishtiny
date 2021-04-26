#include <ratio>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/countdown/Counter.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"

TEST_CASE("Test CountdownProgresBar<Counter>", "[nproc:1]") {

  using bar_t = uitsl::ProgressBar<uitsl::Counter>;

  for ( [[maybe_unused]] auto it : bar_t{ std::mega::num })
    ;

}

TEST_CASE("Test CountdownProgresBar<Timer>", "[nproc:1]") {

  using timer_t = uitsl::Timer<
    std::chrono::duration<double, std::ratio<1>>,
    uitsl::CoarseClock
  >;
  using bar_t = uitsl::ProgressBar<timer_t>;

  for ( [[maybe_unused]] auto it : bar_t{ std::chrono::seconds{3} })
    ;

}
