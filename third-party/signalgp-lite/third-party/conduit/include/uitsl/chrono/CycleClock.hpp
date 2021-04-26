#pragma once
#ifndef UITSL_CHRONO_CYCLECLOCK_HPP_INCLUDE
#define UITSL_CHRONO_CYCLECLOCK_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <ctime>
#include <ratio>

#include <benchmark/src/cycleclock.h>

#include "chrono_utils.hpp"
#include "cycle_freq.hpp"

// quick-bench profiling results
// gcc: https://quick-bench.com/q/WcbqUWBCoNBJvCP4n8h3kYfZDXU
// clang: https://quick-bench.com/q/ezQiwgeEWnKsYgcf3yZHJ1arjwQ
// ~77x faster than std::chrono clocks

namespace uitsl {

struct CycleClock {

  // An arithmetic type or a class emulating an arithmetic type.
  // The representation type of C1::duration.
  using rep = size_t;

  // A specialization of std::ratio.
  // The tick period of the clock in seconds.
  using period = std::nano;

  // The duration type of the clock.
  using duration = std::chrono::duration<rep, period>;

  // The std::chrono::time_point type of the clock.
  using time_point = std::chrono::time_point<CycleClock>;

  // true if t1 <= t2 is always true and the time between clock ticks
  // is constant, otherwise false
  const static bool is_steady{ false };

  // Returns a time_point object representing the current point in time.
  static time_point now() {
    return time_point{
      std::chrono::nanoseconds{ static_cast<size_t>( std::round(
        benchmark::cycleclock::Now() / uitsl::get_cycle_freq()
      ) ) }
    };
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_CHRONO_CYCLECLOCK_HPP_INCLUDE
