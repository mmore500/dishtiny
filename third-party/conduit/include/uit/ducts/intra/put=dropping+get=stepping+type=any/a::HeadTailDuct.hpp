#pragma once
#ifndef UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__HEADTAILDUCT_HPP_INCLUDE
#define UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__HEADTAILDUCT_HPP_INCLUDE

#include <stddef.h>
#include <string>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../uitsl/debug/occupancy_audit.hpp"
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
class HeadTailDuct {

  using T = typename ImplSpec::T;
  static_assert( uitsl::a::static_test<T>(), uitsl_a_message );
  constexpr inline static size_t N{ImplSpec::N};

  // aligned implicit value initializes T
  using buffer_t = emp::array<T, N>;

  size_t head{};
  size_t tail{};
  buffer_t buffer;

  uitsl_occupancy_auditor;

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() const {
    //TODO FIXME handle wraparound case?
    emp_assert(tail <= head);
    return head - tail;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void DoPut(const T& val) {
    uitsl_occupancy_audit(1);
    ++head;
    buffer[head % N] = val;
    emp_assert( CountUnconsumedGets() <= N );
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  void DoPut(P&& val) {
    uitsl_occupancy_audit(1);
    ++head;
    buffer[head % N] = std::forward<P>(val);
    emp_assert( CountUnconsumedGets() <= N );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() const { return CountUnconsumedGets() < N; }

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (IsReadyForPut()) { DoPut(val); return true; }
    else return false;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    if (IsReadyForPut()) { DoPut(std::forward<P>(val)); return true; }
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
  size_t TryConsumeGets(const size_t n) {
    uitsl_occupancy_audit(1);
    const size_t num_consumed = std::min( CountUnconsumedGets(), n );
    tail += num_consumed;
    return num_consumed;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return buffer[tail % N]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return buffer[tail % N]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "HeadTailDuct"; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("buffer_t buffer", buffer[0]) << '\n';
    ss << uitsl::format_member("size_t head", head);
    ss << uitsl::format_member("size_t tail", tail);
    return ss.str();
  }


};

} // namespace a
} // namespace uit

#endif // #ifndef UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__HEADTAILDUCT_HPP_INCLUDE
