#pragma once
#ifndef UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_IMPL_PENDINGDUCT_HPP_INCLUDE
#define UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_IMPL_PENDINGDUCT_HPP_INCLUDE

#include <stddef.h>
#include <string>

#include "../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../uitsl/debug/occupancy_audit.hpp"
#include "../../../../../uitsl/meta/a::static_test.hpp"
#include "../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../uitsl/utility/print_utils.hpp"

namespace uit {
namespace internal {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<
  typename PendingType,
  typename BufferElementType,
  typename ImplSpec
>
class PendingDuct {

  using T = typename ImplSpec::T;
  static_assert( uitsl::a::static_test<T>(), uitsl_a_message );
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<BufferElementType, N>;

  PendingType pending_gets{};
  uitsl::CircularIndex<N> put_position{1};
  uitsl::CircularIndex<N> get_position{};
  buffer_t buffer{};

  uitsl_occupancy_auditor;

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() const { return pending_gets; }

  void DoPut(const T& val) {
    uitsl_occupancy_audit(1);
    buffer[put_position] = val;
    ++pending_gets;
    ++put_position;
    emp_assert( pending_gets <= N );
  }

  template<typename P>
  void DoPut(P&& val) {
    uitsl_occupancy_audit(1);
    buffer[put_position] = std::forward<P>(val);
    ++pending_gets;
    ++put_position;
    emp_assert( pending_gets <= N );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() const { return pending_gets < N; }

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
  bool Put(P&& val) {
    if (IsReadyForPut()) { DoPut( std::forward<P>(val) ); return true; }
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
   * @param requested.
   * @return num consumed.
   */
  size_t TryConsumeGets(const size_t requested) {
    uitsl_occupancy_audit(1);
    const size_t num_consumed = std::min( requested, CountUnconsumedGets() );
    get_position += num_consumed;
    pending_gets -= num_consumed;
    return num_consumed;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return buffer[get_position]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return buffer[get_position]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "PendingDuct"; }

  static constexpr bool CanStep() { return true; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("buffer_t buffer", buffer[0]) << '\n';
    ss << uitsl::format_member("pending_t pending_gets", pending_gets);
    ss << uitsl::format_member("pending_t get_position", get_position);
    return ss.str();
  }


};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_IMPL_PENDINGDUCT_HPP_INCLUDE
