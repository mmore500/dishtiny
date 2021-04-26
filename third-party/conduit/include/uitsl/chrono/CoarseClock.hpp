#pragma once
#ifndef UITSL_CHRONO_COARSECLOCK_HPP_INCLUDE
#define UITSL_CHRONO_COARSECLOCK_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <ctime>
#include <ratio>

#include "chrono_utils.hpp"

// quick-bench profiling results
// gcc: https://quick-bench.com/q/cuGAJ2gORfc2KnG43IYElknSBeE
// clang: https://quick-bench.com/q/wq_AmWxlB370-9kWRvqYOW9s094
// ~280x faster than std::chrono clocks

namespace uitsl {

struct CoarseClock {

  // An arithmetic type or a class emulating an arithmetic type.
  // The representation type of C1::duration.
  using rep = std::time_t;

  // A specialization of std::ratio.
  // The tick period of the clock in seconds.
  using period = std::ratio<1, 1>;

  // The duration type of the clock.
  using duration = std::chrono::duration<rep, period>;

  // The std::chrono::time_point type of the clock.
  using time_point = std::chrono::time_point<CoarseClock>;

  // true if t1 <= t2 is always true and the time between clock ticks
  // is constant, otherwise false
  const static bool is_steady{ false };

  // Returns a time_point object representing the current point in time.
  static time_point now() {
    return time_point{
      std::chrono::seconds{std::time(NULL)}
    };
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_CHRONO_COARSECLOCK_HPP_INCLUDE
