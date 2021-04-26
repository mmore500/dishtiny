#pragma once
#ifndef UITSL_CHRONO_COARSEREALCLOCK_HPP_INCLUDE
#define UITSL_CHRONO_COARSEREALCLOCK_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <ratio>
#include <time.h>

#include "chrono_utils.hpp"

// quick-bench profiling results
// gcc: https://quick-bench.com/q/cxJOR_fpRBIh7pibCuLST5Za6Vo
// clang: https://quick-bench.com/q/nB-82RwuxT7re-M3__Jfkcoa2DE
// ~100x faster than std::chrono clocks
// clock resolution seems to be ~4ms
// gcc: https://godbolt.org/z/71aKzY
// clang: https://godbolt.org/z/Y1scbr

namespace uitsl {

struct CoarseRealClock {

  // An arithmetic type or a class emulating an arithmetic type.
  // The representation type of C1::duration.
  using rep = size_t;

  // A specialization of std::ratio.
  // The tick period of the clock in seconds.
  using period = std::milli;

  // The duration type of the clock.
  using duration = std::chrono::duration<rep, period>;

  // The std::chrono::time_point type of the clock.
  using time_point = std::chrono::time_point<CoarseRealClock>;

  // true if t1 <= t2 is always true and the time between clock ticks
  // is constant, otherwise false
  const static bool is_steady{ false };

  // Returns a time_point object representing the current point in time.
  static time_point now() {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME_COARSE, &spec);

    return time_point{
      std::chrono::milliseconds{
        spec.tv_sec * std::milli::den
        + spec.tv_nsec * (std::milli::den / std::nano::den)
      }
    };
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_CHRONO_COARSEREALCLOCK_HPP_INCLUDE
