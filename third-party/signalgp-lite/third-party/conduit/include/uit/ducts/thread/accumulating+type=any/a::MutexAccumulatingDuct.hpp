#pragma once
#ifndef UIT_DUCTS_THREAD_ACCUMULATING_TYPE_ANY_A__MUTEXACCUMULATINGDUCT_HPP_INCLUDE
#define UIT_DUCTS_THREAD_ACCUMULATING_TYPE_ANY_A__MUTEXACCUMULATINGDUCT_HPP_INCLUDE

#include <limits>
#include <mutex>
#include <stddef.h>
#include <string>
#include <utility>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../uitsl/meta/a::static_test.hpp"
#include "../../../../uitsl/utility/print_utils.hpp"

namespace uit {
namespace a {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class MutexAccumulatingDuct {

  using T = typename ImplSpec::T;
  static_assert( uitsl::a::static_test<T>(), uitsl_a_message );

  T accumulator{};
  T cache{};

  size_t updates_since_last_get{};

  mutable std::mutex mutex;

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    const std::lock_guard guard{ mutex };
    ++updates_since_last_get;
    accumulator += val;
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  /**
   * TODO.
   *
   * @param requested TODO.
   */
  size_t TryConsumeGets(const size_t requested) {
    emp_assert( requested == std::numeric_limits<size_t>::max() );
    const std::lock_guard guard{ mutex };
    cache = std::exchange(accumulator, T{});
    emp_assert( accumulator == T{} );
    return std::exchange( updates_since_last_get, 0 );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return cache; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return cache; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "MutexAccumulatingDuct"; }

  static constexpr bool CanStep() { return false; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    return ss.str();
  }


};

} // namespace a
} // namespace uit

#endif // #ifndef UIT_DUCTS_THREAD_ACCUMULATING_TYPE_ANY_A__MUTEXACCUMULATINGDUCT_HPP_INCLUDE
