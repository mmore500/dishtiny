#pragma once
#ifndef UITSL_POLYFILL_LATCH_HPP_INCLUDE
#define UITSL_POLYFILL_LATCH_HPP_INCLUDE

#include <atomic>
#include <condition_variable>
#include <cstddef>

// polyfill until C++20 barrier becomes available
// TODO C++20 cpp20 switch to std::barrier

// adapted from
// https://github.com/llvm/llvm-project/blob/bcf14f375e29b94e7abb381920df795eeefb2309/libcxx/include/latch

namespace std {

class latch
{
  ptrdiff_t arrived;

  mutable std::condition_variable cv;
  mutable std::mutex mutex;

public:
  static constexpr ptrdiff_t max() noexcept {
    return std::numeric_limits<ptrdiff_t>::max();
  }

  inline explicit latch(ptrdiff_t expected)
  : arrived(expected)
  { ; }

  ~latch() = default;
  latch(const latch&) = delete;
  latch& operator=(const latch&) = delete;

  inline void count_down(ptrdiff_t update = 1) {

    const std::lock_guard<std::mutex> lock{mutex};

    arrived -= update;

    if (try_wait()) cv.notify_all();

  }

  inline bool try_wait() const noexcept { return arrived == 0; }

  inline void wait() const {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this](){ return try_wait(); });
  }

  inline void arrive_and_wait(ptrdiff_t update = 1) {
    count_down(update);
    wait();
  }
};

} // namespace uitsl

#endif // #ifndef UITSL_POLYFILL_LATCH_HPP_INCLUDE
