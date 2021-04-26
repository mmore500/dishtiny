#pragma once
#ifndef UITSL_COUNTDOWN_JOBTIMER_HPP_INCLUDE
#define UITSL_COUNTDOWN_JOBTIMER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <stddef.h>
#include <string>

#include "../chrono/chrono_utils.hpp"

#include "impl/CountdownIterator.hpp"

namespace uitsl {

template<
  typename Duration_T=std::chrono::duration<double, std::ratio<1>>,
  typename Clock_T=std::chrono::steady_clock
>
class JobTimer {

  using time_point_t = std::chrono::time_point<Clock_T>;

  Duration_T duration;

  time_point_t start{
    Clock_T::now() - std::chrono::seconds{
      std::stoi( std::getenv("SECONDS") ?: "0" )
    }
  };

public:

  using iterator = uitsl::CountdownIterator<JobTimer>;
  using elapsed_t = Duration_T;

  JobTimer(
    const Duration_T& duration_=infinite_duration
  ) : duration(duration_)
  { ; }

  bool IsComplete() const { return duration <= GetElapsed(); }

  Duration_T GetElapsed() const {
    return std::chrono::duration_cast<Duration_T>(
      Clock_T::now() - start
    );
  }

  Duration_T GetRemaining() const {
    return std::max(
      duration - GetElapsed(),
      Duration_T{0}
    );
  }

  double GetFractionComplete() const {
    return (
      static_cast<double>( GetElapsed().count() )
      / static_cast<double>( duration.count() )
    );
  }

  JobTimer& operator++() { return *this; }

  iterator begin() { return iterator{ *this }; }

  iterator end() { return iterator{}; }

};

} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_JOBTIMER_HPP_INCLUDE
