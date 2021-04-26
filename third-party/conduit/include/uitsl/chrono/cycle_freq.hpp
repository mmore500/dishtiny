#pragma once
#ifndef UITSL_CHRONO_CYCLE_FREQ_HPP_INCLUDE
#define UITSL_CHRONO_CYCLE_FREQ_HPP_INCLUDE

#include <chrono>
#include <thread>

#include <benchmark/src/cycleclock.h>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "TimeGuard.hpp"

namespace uitsl {

  // estimate cycles per nanosecond
  inline double measure_cycle_freq() {

    using time_guard_t = uitsl::TimeGuard<
      std::chrono::nanoseconds,
      std::chrono::high_resolution_clock
    >;

    size_t diff;
    std::chrono::nanoseconds ns; { const time_guard_t guard(ns);
      const auto before_cycles{ benchmark::cycleclock::Now() };

      std::this_thread::sleep_for( std::chrono::milliseconds{ 1 } );

      const auto after_cycles{ benchmark::cycleclock::Now() };
      diff = after_cycles - before_cycles;
    }

    return diff / static_cast<double>( ns.count() );

  }

  inline double estimate_cycle_freq() {

    emp::vector<double> readings;

    std::generate_n(
      std::back_inserter(readings),
      100,
      measure_cycle_freq
    );

    return std::accumulate(
      std::begin(readings),
      std::end(readings),
      double{}
    ) / readings.size();

  }

  inline double get_cycle_freq() {
    const static double res{ estimate_cycle_freq() };
    return res;
  }

} // namespace uitsl

#endif // #ifndef UITSL_CHRONO_CYCLE_FREQ_HPP_INCLUDE
