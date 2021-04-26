#pragma once
#ifndef UIT_DUCTS_INTRA_PUT_GROWING_GET_SKIPPING_TYPE_ANY_A__SCONCEDUCT_HPP_INCLUDE
#define UIT_DUCTS_INTRA_PUT_GROWING_GET_SKIPPING_TYPE_ANY_A__SCONCEDUCT_HPP_INCLUDE

#include <limits>
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
class SconceDuct {

  using T = typename ImplSpec::T;

  T sconce{};
  static_assert( uitsl::a::static_test<T>(), uitsl_a_message );
  size_t updates_since_last_get{};

  size_t CountUnconsumedGets() const { return updates_since_last_get; }

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    sconce = val;
    ++updates_since_last_get;
    return true;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(T&& val) {
    sconce = std::forward<P>(val);
    ++updates_since_last_get;
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

    return std::exchange(updates_since_last_get, 0);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return sconce; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return sconce; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "SconceDuct"; }

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

#endif // #ifndef UIT_DUCTS_INTRA_PUT_GROWING_GET_SKIPPING_TYPE_ANY_A__SCONCEDUCT_HPP_INCLUDE
