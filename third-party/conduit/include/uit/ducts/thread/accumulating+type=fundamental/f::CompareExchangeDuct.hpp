#pragma once
#ifndef UIT_DUCTS_THREAD_ACCUMULATING_TYPE_FUNDAMENTAL_F__COMPAREEXCHANGEDUCT_HPP_INCLUDE
#define UIT_DUCTS_THREAD_ACCUMULATING_TYPE_FUNDAMENTAL_F__COMPAREEXCHANGEDUCT_HPP_INCLUDE

#include <limits>
#include <stddef.h>
#include <string>
#include <utility>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../uitsl/meta/f::static_test.hpp"
#include "../../../../uitsl/parallel/AtomicOp.hpp"
#include "../../../../uitsl/parallel/RelaxedAtomic.hpp"
#include "../../../../uitsl/utility/print_utils.hpp"

namespace uit {
namespace f {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class CompareExchangeDuct {

  using T = typename ImplSpec::T;
  static_assert( uitsl::f::static_test<T>(), uitsl_f_message );

  uitsl::RelaxedAtomic<T> accumulator{};
  T cache{};

  size_t updates_since_last_get{};

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    ++updates_since_last_get;
    uitsl::AtomicOp<std::plus<>>(accumulator, val);
    return true;
  }

  /**
   * TODO.
   *
   * @param requested TODO.
   */
  size_t TryConsumeGets(const size_t requested) {
    emp_assert( requested == std::numeric_limits<size_t>::max() );
    cache = accumulator.exchange( T{} );
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
  static std::string GetType() { return "CompareExchangeDuct"; }

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

} // namespace f
} // namespace uit

#endif // #ifndef UIT_DUCTS_THREAD_ACCUMULATING_TYPE_FUNDAMENTAL_F__COMPAREEXCHANGEDUCT_HPP_INCLUDE
