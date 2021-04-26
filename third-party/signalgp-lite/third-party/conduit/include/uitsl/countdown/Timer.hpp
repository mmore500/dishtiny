#pragma once
#ifndef UITSL_COUNTDOWN_TIMER_HPP_INCLUDE
#define UITSL_COUNTDOWN_TIMER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <stddef.h>

#include "../chrono/chrono_utils.hpp"
#include "../chrono/CoarseClock.hpp"
#include "../chrono/CoarseRealClock.hpp"

#include "impl/CountdownIterator.hpp"

namespace uitsl {

template<
  typename Duration_T=std::chrono::duration<double, std::ratio<1>>,
  typename Clock_T=std::chrono::steady_clock
>
class Timer {

  Duration_T duration;

  std::chrono::time_point<Clock_T> start{ Clock_T::now() };

public:

  using iterator = uitsl::CountdownIterator<Timer>;
  using elapsed_t = Duration_T;

  Timer(
    const Duration_T& duration_=infinite_duration
  ) : duration(duration_)
  { ; }

  Timer( const double duration_ ) : Timer( Duration_T{ duration_ } )
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

  Timer& operator++() { return *this; }

  iterator begin() { return iterator{ *this }; }

  iterator end() { return iterator{}; }

  void Reset() { start =  Clock_T::now(); }

};

// convenience typedef
using CoarseTimer = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;

using CoarseRealTimer = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseRealClock
>;


} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_TIMER_HPP_INCLUDE
