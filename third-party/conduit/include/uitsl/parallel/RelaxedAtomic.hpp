#pragma once
#ifndef UITSL_PARALLEL_RELAXEDATOMIC_HPP_INCLUDE
#define UITSL_PARALLEL_RELAXEDATOMIC_HPP_INCLUDE

#include <atomic>

namespace uitsl {

template <typename T>
class RelaxedAtomic : public std::atomic<T> {

  using base_t = std::atomic<T>;

public:

  // pre-increment
  T operator++() noexcept {
    return base_t::fetch_add(1, std::memory_order_relaxed) + 1;
  }

  // pre-decrement
  T operator--() noexcept {
    return base_t::fetch_sub(1, std::memory_order_relaxed) - 1;
  }

  // post-increment
  T operator++(int) noexcept {
    return base_t::fetch_add(1, std::memory_order_relaxed);
  }

  // post-decrement
  T operator--(int) noexcept {
    return base_t::fetch_sub(1, std::memory_order_relaxed);
  }

  T operator+=(const T arg) noexcept {
    return base_t::fetch_add(arg, std::memory_order_relaxed) + arg;
  }

  T operator-=(const T arg) noexcept {
    return base_t::fetch_sub(arg, std::memory_order_relaxed) - arg;
  }

  T operator&=(const T arg) noexcept {
    return base_t::fetch_and(arg, std::memory_order_relaxed) & arg;
  }

  T operator|=(const T arg) noexcept {
    return base_t::fetch_or(arg, std::memory_order_relaxed) | arg;
  }

  T operator^=(const T arg) noexcept {
    return base_t::fetch_xor(arg, std::memory_order_relaxed) ^ arg;
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_RELAXEDATOMIC_HPP_INCLUDE
