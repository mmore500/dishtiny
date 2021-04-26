#pragma once
#ifndef UITSL_COUNTDOWN_PERIODICREFRESHTIMER_HPP_INCLUDE
#define UITSL_COUNTDOWN_PERIODICREFRESHTIMER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <stddef.h>

#include "../chrono/chrono_utils.hpp"

#include "impl/CountdownIterator.hpp"

namespace uitsl {

template<
  typename Duration_T=std::chrono::duration<double, std::ratio<1>>,
  typename Clock_T=std::chrono::steady_clock
>
class PeriodicRefreshTimer {

  const Duration_T duration;

  const std::chrono::time_point<Clock_T> start{ Clock_T::now() };

  const size_t refresh_freq;
  mutable size_t refresh_counter{0};

public:

  using iterator = uitsl::CountdownIterator<PeriodicRefreshTimer>;
  using elapsed_t = Duration_T;

  PeriodicRefreshTimer(
    const Duration_T& duration_=infinite_duration,
    const size_t refresh_freq=1
  ) : duration(duration_)
  , refresh_freq(refresh_freq)
  { ; }

  bool IsComplete() const {
    if (refresh_counter == std::numeric_limits<size_t>::max()) return true;
    else {
      const bool res = refresh_counter ? false : duration <= GetElapsed();
      refresh_counter = res
        ? std::numeric_limits<size_t>::max()
        : (refresh_counter + 1) % refresh_freq
      ;
      return res;
    }
  }

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

  PeriodicRefreshTimer& operator++() { return *this; }

  iterator begin() { return iterator{ *this }; }

  iterator end() { return iterator{}; }

};

} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_PERIODICREFRESHTIMER_HPP_INCLUDE
