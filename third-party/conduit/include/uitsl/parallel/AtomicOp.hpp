#pragma once
#ifndef UITSL_PARALLEL_ATOMICOP_HPP_INCLUDE
#define UITSL_PARALLEL_ATOMICOP_HPP_INCLUDE

#include <atomic>

namespace uitsl {

// adapted from https://www.reddit.com/r/cpp/comments/338pcj/atomic_addition_of_floats_using_compare_exchange/
// and https://wandbox.org/permlink/aRqqxVAFzWdB8y0w
// see also https://stackoverflow.com/a/25207431

namespace internal {
template <typename T>
  struct identity {
    using type = T;
  };
  template <typename T>
  using identity_t = typename identity<T>::type;
}

template<typename BinaryOp, typename T>
T AtomicOp(
  std::atomic<T> &target,
  internal::identity_t<T> const operand,
  BinaryOp op = BinaryOp{}
) {
  T old = target;
  T desired;
  do {
    desired = op(old, operand);
  } while ( !target.compare_exchange_weak(old, desired) );
  return desired;
}

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_ATOMICOP_HPP_INCLUDE
