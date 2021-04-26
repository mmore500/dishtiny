#pragma once
#ifndef UIT_SPOUTS_INLET_HPP_INCLUDE
#define UIT_SPOUTS_INLET_HPP_INCLUDE

#include <atomic>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stddef.h>
#include <utility>

#include "../../uitsl/debug/occupancy_audit.hpp"
#include "../../uitsl/nonce/CircularIndex.hpp"
#include "../../uitsl/utility/print_utils.hpp"

#include "../ducts/Duct.hpp"

namespace uit {

/**
 * Input to conduit transmission.
 *
 * Allows user to initiate
 *
 *  - potentially-blocking, assured transmisison via `Put`, or
 *  - non-blocking, potentially-dropped transmission via `TryPut`.
 *
 * An `Inlet` holds a `std::shared_ptr` to a `Duct` object, which manages data
 * transmission from the `Inlet`.
 *
 * An `Inlet`'s underlying `Duct` may be altered or replaced at runtime, for
 * example to provide thread-safe or process-safe transmission.
 *
 * - `EmplaceDuct` emplaces a new transmission implementation within
 *   the  existing `Duct` object. (See `include/ducts/Duct.hpp` for details.)
 * - `SplitDuct` makes a new `Duct` and points the `Inlet`'s `std::shared_ptr`
 *   to that `Duct`.
 *
 * If an `Outlet` holds a `std::shared_ptr` to the `Inlet`'s `Duct`, under an
 * `EmplaceDuct` call the `Duct`'s change in transmission implementation will
 * be visible to the `Outlet` and `Inlet` and the `Outlet` will still share a
 * `Duct`. However, under a `SplitDuct` call that `Outlet`'s `Duct` will be
 * unaffected. After a `SplitDuct` call, the `Inlet` and `Outlet` will hold
 * `std::shared_ptr`'s to separate `Duct`s.
 *
 * @tparam ImplSpec class with static and typedef members specifying
 *   implementation details for the conduit framework. See
 *   `include/config/ImplSpec.hpp`.
 *
 * @note End users should probably never have to directly instantiate this
 *   class. The `Conduit`, `Sink`, and `Source` classes take care of creating a
 *   `Duct` and tying it to an `Inlet` and/or `Outlet`. Better yet, the
 *   `MeshTopology` interface allows end users to construct a conduit network
 *    in terms of a connection topology and a mapping to assign nodes to
 *    threads and processes without having to manually construct `Conduits` and
 *    emplace necessary thread-safe and/or process-safe `Duct` implementations.
 */
template<typename ImplSpec_>
class Inlet {

public:
  using ImplSpec = ImplSpec_;

private:
  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ ImplSpec::N };

  using index_t = uitsl::CircularIndex<N>;

  /// TODO.
  using duct_t = internal::Duct<ImplSpec>;
  std::shared_ptr<duct_t> duct;

  /// How many put operations have been performed?
  size_t attempted_put_count{};

  /// How many times has Put blocked?
  size_t blocked_put_count{};

  // How many TryPut calls have dropped?
  size_t dropped_put_count{};

  uitsl_occupancy_auditor;

  /**
   * TODO.
   *
   * @param val
   */
  bool DoTryPut(const T& val) { return duct->TryPut(val); }

  /**
   * TODO.
   *
   * @param val
   */
  template<typename P>
  bool DoTryPut(P&& val) { return duct->TryPut(std::forward<P>(val)); }

public:

  /**
   * TODO.
   *
   * @param duct_ TODO.
   */
  Inlet(
    std::shared_ptr<duct_t> duct_
  ) : duct(duct_) { ; }

  // potentially blocking
  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    uitsl_occupancy_audit(1);

    ++attempted_put_count;
    bool was_blocked{ false };
    while (!DoTryPut(val)) was_blocked = true;

    blocked_put_count += was_blocked;

  }

  // non-blocking
  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    uitsl_occupancy_audit(1);

    ++attempted_put_count;
    if ( DoTryPut(val) ) return true;
    else { ++dropped_put_count; return false; }

  }

  // non-blocking
  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    uitsl_occupancy_audit(1);

    ++attempted_put_count;
    if ( DoTryPut(std::forward<P>(val)) ) return true;
    else { ++dropped_put_count; return false; }

  }

  /**
   * TODO.
   *
   */
  bool TryFlush() { return duct->TryFlush(); }

  /**
   * TODO.
   *
   */
  void Flush() { while( !TryFlush() ); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetAttemptedPutCount() const { return attempted_put_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetBlockedPutCount() const { return blocked_put_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetDroppedPutCount() const { return dropped_put_count; }

  /**
   * TODO.
   *
   * @tparam WhichDuct
   * @tparam Args
   * @param args
   */
  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    duct->template EmplaceImpl<WhichDuct>(std::forward<Args>(args)...);
  }

  /**
   * TODO.
   *
   * @tparam WhichDuct
   * @tparam Args
   * @param args
   */
  template <typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    duct = std::make_shared<duct_t>(
      std::in_place_type_t<WhichDuct>{},
      std::forward<Args>(args)...
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  typename duct_t::uid_t GetDuctUID() const { return duct->GetUID(); }

  emp::optional<bool> HoldsIntraImpl() const { return duct->HoldsIntraImpl(); }

  emp::optional<bool> HoldsThreadImpl() const {
    return duct->HoldsThreadImpl();
  }

  emp::optional<bool> HoldsProcImpl() const { return duct->HoldsProcImpl(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << uitsl::format_member("duct_t duct", *duct) << '\n';
    ss << uitsl::format_member(
      "size_t attempted_put_count",
      attempted_put_count
    ) << '\n';
    ss << uitsl::format_member(
      "size_t dropped_put_count",
      dropped_put_count
    ) << '\n';
    ss << uitsl::format_member(
      "size_t blocked_put_count",
      blocked_put_count
    );
    return ss.str();
  }


};

} // namespace uit

#endif // #ifndef UIT_SPOUTS_INLET_HPP_INCLUDE
