#pragma once
#ifndef UITSL_CHRONO_CLOCKDELTADETECTOR_HPP_INCLUDE
#define UITSL_CHRONO_CLOCKDELTADETECTOR_HPP_INCLUDE

#include <utility>

#include "CoarseClock.hpp"

namespace uitsl {

template<typename Clock=uitsl::CoarseClock>
class ClockDeltaDetector {

  using time_point = typename Clock::time_point;

  time_point last_checked{ Clock::now() };

public:

  bool HasDeltaElapsed() {

    const auto now = Clock::now();
    return std::exchange( last_checked, now ) != now;

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_CHRONO_CLOCKDELTADETECTOR_HPP_INCLUDE
