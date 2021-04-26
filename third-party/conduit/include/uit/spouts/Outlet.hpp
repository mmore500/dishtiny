#pragma once
#ifndef UIT_SPOUTS_OUTLET_HPP_INCLUDE
#define UIT_SPOUTS_OUTLET_HPP_INCLUDE

#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <stddef.h>
#include <utility>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../../uitsl/debug/occupancy_audit.hpp"
#include "../../uitsl/nonce/CircularIndex.hpp"
#include "../../uitsl/parallel/thread_utils.hpp"

#include "../ducts/Duct.hpp"

namespace uit {

/**
 * Output from conduit transmission.
 *
 * Allows user to initiate
 *
 *  - potentially-blocking, strictly-sequential of the next unread received
 *     transmission, or
 *  - non-blocking fetch of the latest received transmission via `JumpGet`.
 *
 * An `Outlet` holds a `std::shared_ptr` to a `Duct` object, which manages data
 * transmission to the `Outlet`.
 *
 * An `Outlet`'s underlying `Duct` may be altered or replaced at runtime, for
 * example to provide thread-safe or process-safe transmission.
 *
 * - `EmplaceDuct` emplaces a new transmission implementation within
 *   the  existing `Duct` object. (See `include/ducts/Duct.hpp` for details.)
 * - `SplitDuct` makes a new `Duct` and points the `Outlet`'s `std::shared_ptr`
 *   to that `Duct`.
 *
 * If an `Inlet` holds a `std::shared_ptr` to the `Outlet`'s `Duct`, under an
 * `EmplaceDuct` call the `Duct`'s change in transmission implementation will
 * be visible to the `Inlet` and the `Inlet` and `Outlet` will still share a
 * `Duct`. However, under a `SplitDuct` call that `Inlet`'s `Duct` will be
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
class Outlet {

public:
  using ImplSpec = ImplSpec_;

private:
  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using index_t = uitsl::CircularIndex<N>;

  using duct_t = internal::Duct<ImplSpec>;
  std::shared_ptr<duct_t> duct;

  // TODO move this to ImplSpec?
  static_assert(N > 0);

  /// How many times has outlet been read from?
  mutable size_t read_count{0};

  /// How many times has current value changed?
  size_t revision_count{0};

  /// Total distance traversed through underlying buffer.
  size_t net_flux{0};

  uitsl_occupancy_auditor;

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t TryConsumeGets(const size_t n) {
    uitsl_occupancy_audit(1);
    return LogStep( duct->TryConsumeGets(n) );
  }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t LogStep(const size_t n) {
    revision_count += (n > 0);
    net_flux += n;
    return n;
  }

  void LogRead() const { ++read_count; }

public:

  /**
   * TODO.
   */
  Outlet(
    std::shared_ptr<duct_t> duct_
  ) : duct(duct_) { ; }

  size_t TryStep(const size_t num_steps=1) {
    return TryConsumeGets(num_steps);
  }

  size_t Jump() {
    return TryConsumeGets( std::numeric_limits<size_t>::max() );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { LogRead(); return std::as_const(*duct).Get(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { LogRead(); return duct->Get(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& JumpGet() {
    uitsl_occupancy_audit(1);
    Jump();
    return Get();
  }

  /**
   * Get next received value.
   *
   * Blocking.
   *
   * @return TODO.
   */
  const T& GetNext() {
    uitsl_occupancy_audit(1);
    while (TryStep() == 0);
    return Get();
  }



  using optional_ref_t = emp::optional<std::reference_wrapper<const T>>;

  /**
   * Get next if available.
   *
   * Non-blocking.
   *
   * @return TODO.
   */
   optional_ref_t GetNextOrNullopt() {
    uitsl_occupancy_audit(1);
    return TryStep()
      ? optional_ref_t{ std::reference_wrapper{ Get() } }
      : std::nullopt;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetReadCount() const { return read_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetRevisionCount() const { return revision_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNetFlux() const { return net_flux; }

  /**
   * TODO.
   *
   * @tparam WhichDuct TODO
   * @tparam Args TODO
   * @param args TODO
   */
  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    duct->template EmplaceImpl<WhichDuct>(std::forward<Args>(args)...);
  }

  /**
   * TODO.
   *
   * @tparam WhichDuct TODO
   * @tparam Args TODO
   * @param args TODO
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

  bool CanStep() const { return duct->CanStep(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << uitsl::format_member("std::shared_ptr<duct_t> duct", *duct) << '\n';
    ss << uitsl::format_member("size_t read_count", read_count) << '\n';
    ss << uitsl::format_member("size_t revision_count", revision_count) << '\n';
    ss << uitsl::format_member("size_t net_flux", net_flux);
    return ss.str();
  }

};

} // namespace uit

#endif // #ifndef UIT_SPOUTS_OUTLET_HPP_INCLUDE
