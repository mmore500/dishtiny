#pragma once
#ifndef UITSL_POLYFILL_BARRIER_HPP_INCLUDE
#define UITSL_POLYFILL_BARRIER_HPP_INCLUDE

#include <condition_variable>
#include <cstddef>

// polyfill until C++20 barrier becomes available
// TODO C++20 cpp20 switch to std::barrier

namespace std {

// adapted from
// https://github.com/llvm/llvm-project/blob/bcf14f375e29b94e7abb381920df795eeefb2309/libcxx/include/barrier

struct EmptyCompletion
{
  void operator()() noexcept { ; }
};

/*
The alternative implementation of __barrier_base is a central barrier.
Two versions of this algorithm are provided:
 1. A fairly straightforward implementation of the litterature for the
  general case where the completion function is not empty.
 2. An optimized implementation that exploits 2's complement arithmetic
  and well-defined overflow in atomic arithmetic, to handle the phase
  roll-over for free.
*/

template<class OnCompletion=EmptyCompletion>
class barrier {

public:
  using arrival_token = bool;

private:

  ptrdiff_t expected;
  ptrdiff_t arrived;
  OnCompletion on_completion;

  bool phase{false};

  mutable std::condition_variable cv;
  mutable std::mutex mutex;

  arrival_token arrive() {

    const std::lock_guard<std::mutex> lock{mutex};

    constexpr ptrdiff_t update{1};

    arrived -= update;
    const auto old_phase = phase;

    if (arrived == 0) {
      on_completion();
      arrived = expected;
      phase = !phase;
      cv.notify_all();
    }

    return old_phase;

  }

public:

  static constexpr ptrdiff_t max() noexcept {
    return std::numeric_limits<ptrdiff_t>::max();
  }

  barrier(
    const ptrdiff_t expected_,
    const OnCompletion on_completion_ = OnCompletion{}
  ) : expected(expected_)
  , arrived(expected_)
  , on_completion(std::move(on_completion_))
  { ; }

  void wait(arrival_token&& old_phase) const {

    std::unique_lock<std::mutex> lock{mutex};
    cv.wait(
      lock,
      [this, old_phase](){ return phase != old_phase; }
    );

  }

  void arrive_and_wait() { wait(arrive()); }

  void arrive_and_drop() {
    {
      const std::lock_guard<std::mutex> lock{mutex};
      --expected;
    }
    arrive();
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_POLYFILL_BARRIER_HPP_INCLUDE
