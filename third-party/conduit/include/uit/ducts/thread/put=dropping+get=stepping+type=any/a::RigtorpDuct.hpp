#pragma once
#ifndef UIT_DUCTS_THREAD_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__RIGTORPDUCT_HPP_INCLUDE
#define UIT_DUCTS_THREAD_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__RIGTORPDUCT_HPP_INCLUDE

#include <mutex>
#include <stddef.h>
#include <string>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/errors.hpp"
#include "../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../../../third-party/SPSCQueue/include/rigtorp/SPSCQueue.h"

#include "../../../../uitsl/debug/occupancy_audit.hpp"
#include "../../../../uitsl/meta/a::static_test.hpp"
#include "../../../../uitsl/parallel/RelaxedAtomic.hpp"
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
class RigtorpDuct {

  using T = typename ImplSpec::T;
  static_assert( uitsl::a::static_test<T>(), uitsl_a_message );
  constexpr inline static size_t N{ImplSpec::N};

  rigtorp::SPSCQueue<T> queue{N};

  using pending_t = uitsl::RelaxedAtomic<size_t>;

  uitsl_occupancy_auditor;

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() const {
    const size_t available = queue.size();
    emp_assert( available );
    return available - 1;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  // TODO why N - 1?
  bool IsReadyForPut() const { return CountUnconsumedGets() < N - 1; }

public:

  RigtorpDuct() { queue.push( T{} ); }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (IsReadyForPut()) { queue.push( val ); return true; }
    else return false;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    if (IsReadyForPut()) { queue.push( std::forward<P>(val) ); return true; }
    else return false;
  }

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
    uitsl_occupancy_audit(1);
    const size_t num_consumed = std::min( requested, CountUnconsumedGets() );
    for (size_t i = 0; i < num_consumed; ++i) queue.pop();
    return num_consumed;

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return *queue.front(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return *queue.front(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "RigtorpDuct"; }

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

#endif // #ifndef UIT_DUCTS_THREAD_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__RIGTORPDUCT_HPP_INCLUDE
