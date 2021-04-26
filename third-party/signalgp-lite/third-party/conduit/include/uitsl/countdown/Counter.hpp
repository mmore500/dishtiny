#pragma once
#ifndef UITSL_COUNTDOWN_COUNTER_HPP_INCLUDE
#define UITSL_COUNTDOWN_COUNTER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <limits>
#include <stddef.h>

#include "../chrono/chrono_utils.hpp"

#include "impl/CountdownIterator.hpp"

namespace uitsl {

class Counter {

  const size_t duration;

  size_t elapsed{};

public:

  using iterator = uitsl::CountdownIterator<Counter>;
  using elapsed_t = size_t;

  explicit Counter(
    const size_t duration_=std::numeric_limits<size_t>::max()
  ) : duration{duration_}
  { ; }

  bool IsComplete() const {
    return GetElapsed() >= duration;
  }

  void Step() { ++elapsed; }

  size_t GetElapsed() const {
    return elapsed;
  }

  size_t GetRemaining() const {
    return GetElapsed() > duration
      ? duration - GetElapsed()
      : 0
    ;
  }

  double GetFractionComplete() const {
    return static_cast<double>(GetElapsed()) / static_cast<double>(duration);
  }

  Counter& operator++() { Step(); return *this; }

  iterator begin() { return iterator{ *this }; }

  iterator end() { return iterator{}; }

};

} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_COUNTER_HPP_INCLUDE
