#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_OUTLET_CACHINGOUTLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_OUTLET_CACHINGOUTLETWRAPPER_HPP_INCLUDE

#include <cstddef>
#include <string>
#include <typeinfo>

#include "../../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../../third-party/Empirical/include/emp/datastructs/QueueCache.hpp"

#include "../../../../uitsl/debug/WarnOnce.hpp"
#include "../../../../uitsl/distributed/CachePacket.hpp"

namespace uit {
namespace internal {

template<typename Outlet>
class CachingOutletWrapper {

  Outlet outlet;

  using ImplSpec = typename Outlet::ImplSpec;

  using this_t = CachingOutletWrapper<Outlet>;
  using value_type = typename ImplSpec::value_type;

  emp::QueueCache<
    size_t,
    value_type,
    ImplSpec::SpoutCacheSize
  > cache;

  void CacheCurrent() {
    const auto& packet = outlet.Get();
    if ( packet.HasData() ) cache.Put( packet.GetID(), packet.GetData() );
    else if ( cache.Contains(packet.GetID()) ) cache.Get( packet.GetID() );
    //^ Get moves item to the front of the cache
    else {
      static const uitsl::WarnOnce w{ "missing cache packet" };
    }
  }

  size_t DoProcTryStep(const size_t num_steps=1) {

    size_t num_steps_countdown{ num_steps };

    while ( num_steps_countdown && outlet.TryStep( 1 ) ) {
      --num_steps_countdown;
      CacheCurrent();
    }

    return num_steps - num_steps_countdown;
  }

  size_t DoProcJump() {
    if ( CanStep() ) return DoProcTryStep( std::numeric_limits<size_t>::max() );
    else {
      const size_t res{ outlet.Jump() };
      CacheCurrent();
      return res;
    }
  }

  const value_type& DoProcGet() const {
    const static value_type pristine{};
    if ( cache.Empty() ) return pristine;
    else return cache.begin()->second;
  }

  value_type& DoProcGet() {
    const static uitsl::WarnOnce w{
      std::string{}
      + "Calling non-const Get on CachingOutletWrapper incurs extra copy, T "
      + typeid( value_type ).name()
      + " ... consider using std::as_const"
    };

    thread_local value_type mutable_copy;
    mutable_copy = const_cast<const this_t*>(this)->DoProcGet();
    return mutable_copy;
  }


public:

  /**
   * Copy constructor.
   */
  CachingOutletWrapper(CachingOutletWrapper& other) = default;

  /**
   * Copy constructor.
   */
  CachingOutletWrapper(const CachingOutletWrapper& other) = default;

  /**
   * Move constructor.
   */
  CachingOutletWrapper(CachingOutletWrapper&& other) = default;

  /**
   * Forwarding constructor.
   */
  template <typename... Args>
  CachingOutletWrapper(Args&&... args)
  : outlet(std::forward<Args>(args)...)
  { ; }

  size_t TryStep(const size_t num_steps) {
    if ( HoldsProcImpl().value_or( true ) ) return DoProcTryStep( num_steps );
    else return outlet.TryStep( num_steps );
  }

  size_t Jump() {
    if ( HoldsProcImpl().value_or( true ) ) return DoProcJump();
    else return outlet.Jump();
  }

  const value_type& Get() const {
    if ( HoldsProcImpl().value_or( true ) ) return DoProcGet();
    else return outlet.Get().GetData();
  }

  value_type& Get() {
    if ( HoldsProcImpl().value_or( true ) ) return DoProcGet();
    else return outlet.Get().GetData();
  }

  const value_type& JumpGet() { Jump(); return Get(); }

  const value_type& GetNext() { while (TryStep() == 0); return Get(); }

  using optional_ref_t = emp::optional<std::reference_wrapper<
    const value_type
  >>;

  optional_ref_t GetNextOrNullopt() {
  return TryStep()
    ? optional_ref_t{ std::reference_wrapper{ Get() } }
    : std::nullopt;
  }

  size_t GetReadCount() const { return outlet.GetReadCount(); }

  size_t GetRevisionCount() const { return outlet.GetRevisionCount(); }

  size_t GetNetFlux() const { return outlet.GetNetFlux(); }

  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    outlet.template EmplaceDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  template <typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    outlet.template SplitDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  auto GetDuctUID() const { return outlet.GetUID(); }

  emp::optional<bool> HoldsIntraImpl() const { return outlet.HoldsIntraImpl(); }

  emp::optional<bool> HoldsThreadImpl() const {
    return outlet.HoldsThreadImpl();
  }

  emp::optional<bool> HoldsProcImpl() const { return outlet.HoldsProcImpl(); }

  bool CanStep() const { return outlet.CanStep(); }

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_OUTLET_CACHINGOUTLETWRAPPER_HPP_INCLUDE
