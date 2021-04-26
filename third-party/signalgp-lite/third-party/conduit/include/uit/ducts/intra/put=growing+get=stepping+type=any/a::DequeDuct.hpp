#pragma once
#ifndef UIT_DUCTS_INTRA_PUT_GROWING_GET_STEPPING_TYPE_ANY_A__DEQUEDUCT_HPP_INCLUDE
#define UIT_DUCTS_INTRA_PUT_GROWING_GET_STEPPING_TYPE_ANY_A__DEQUEDUCT_HPP_INCLUDE

#include <deque>
#include <stddef.h>
#include <string>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/errors.hpp"
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
class DequeDuct {

  using T = typename ImplSpec::T;
  static_assert( uitsl::a::static_test<T>(), uitsl_a_message );

  std::deque<T> queue{ T{} };

  size_t CountUnconsumedGets() const {
    const size_t available = queue.size();
    emp_assert( available );
    return available - 1;
  }

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) { queue.push_back( val ); return true; }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) { queue.push_back( std::forward<P>(val) ); return true; }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t TryConsumeGets(const size_t requested) {
    const size_t num_consumed = std::min( requested, CountUnconsumedGets() );
    queue.erase(
      std::begin(queue),
      std::next(std::begin(queue), num_consumed)
    );
    return num_consumed;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return queue.front(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return queue.front(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "DequeDuct"; }

  static constexpr bool CanStep() { return true; }

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

#endif // #ifndef UIT_DUCTS_INTRA_PUT_GROWING_GET_STEPPING_TYPE_ANY_A__DEQUEDUCT_HPP_INCLUDE
