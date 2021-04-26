#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_INLET_CACHINGINLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_INLET_CACHINGINLETWRAPPER_HPP_INCLUDE

#include <cstddef>
#include <utility>

#include "../../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../../third-party/Empirical/include/emp/datastructs/QueueCache.hpp"

#include "../../../../uitsl/distributed/CachePacket.hpp"

namespace uit {
namespace internal {

template<typename Inlet>
class CachingInletWrapper {

  size_t uid_stepper{ 1 };

  using ImplSpec = typename Inlet::ImplSpec;

  using inlet_t = Inlet;
  inlet_t inlet;

  using value_type = typename ImplSpec::value_type;

  emp::QueueCache<
    value_type,
    size_t,
    ImplSpec::SpoutCacheSize
  > cache;

  void DoProcPut(const value_type& val) {
    bool hit{ true };

    const size_t uid = cache.Get(
      val,
      [this, &hit](const auto& key){ hit = false; return uid_stepper++; }
    );

    if ( hit ) inlet.Put( uitsl::CachePacket<value_type>{ uid } );
    else inlet.Put( uitsl::CachePacket<value_type>{ uid, val } );
  }

  bool DoProcTryPut(const value_type& val) {
    bool hit{ true };

    const size_t uid = cache.Get(
      val,
      [this, &hit](const auto& key){ hit = false; return uid_stepper++; }
    );

    if ( hit ) return inlet.TryPut( uitsl::CachePacket<value_type>{ uid } );
    else {
      const bool res{
        inlet.TryPut( uitsl::CachePacket<value_type>{ uid, val } )
      };
      if ( !res ) cache.Delete( val );
      return res;
    }
  }

  template<typename P>
  bool DoProcTryPut(P&& val) {
    bool hit{ true };

    const size_t uid = cache.Get(
      val,
      [this, &hit](const auto& key){ hit = false; return uid_stepper++; }
    );

    if ( hit ) return inlet.TryPut( uitsl::CachePacket<value_type>{ uid } );
    else {
      const bool res{ inlet.TryPut(
        uitsl::CachePacket<value_type>{ uid, std::forward<P>(val) }
      ) };
      if ( !res ) cache.Delete( val );
      return res;
    }
  }

public:

  /**
   * Copy constructor.
   */
  CachingInletWrapper(CachingInletWrapper& other) = default;

  /**
   * Copy constructor.
   */
  CachingInletWrapper(const CachingInletWrapper& other) = default;

  /**
   * Move constructor.
   */
  CachingInletWrapper(CachingInletWrapper&& other) = default;

  /**
   * Forwarding constructor.
   */
  template <typename... Args>
  CachingInletWrapper(Args&&... args)
  : inlet(std::forward<Args>(args)...)
  { ; }

  void Put(const value_type& val) {
    if ( HoldsProcImpl().value_or(true) ) DoProcPut( val );
    else inlet.Put( uitsl::CachePacket<value_type>{ 0, val } );
  }

  bool TryPut(const value_type& val) {
    if ( HoldsProcImpl().value_or(true) ) return DoProcTryPut( val );
    else return inlet.TryPut( uitsl::CachePacket<value_type>{ 0, val } );
  }

  template<typename P>
  bool TryPut(P&& val) {
    if ( HoldsProcImpl().value_or(true) ) return DoProcTryPut(
      std::forward<P>(val)
    );
    else return inlet.TryPut(
      uitsl::CachePacket<value_type>{ 0, std::forward<P>(val) }
    );

  }

  bool TryFlush() { return inlet.TryFlush(); }

  void Flush() { inlet.Flush(); }

  size_t GetAttemptedPutCount() const { return inlet.GetAttemptedPutCount(); }

  size_t GetBlockedPutCount() const { return inlet.GetBlockedPutCount(); }

  size_t GetDroppedPutCount() const { return inlet.GetDroppedPutCount(); }

  template<typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    inlet.template EmplaceDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  template<typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    inlet.template SplitDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  auto GetDuctUID() const { return inlet.GetUID(); }

  emp::optional<bool> HoldsIntraImpl() const { return inlet.HoldsIntraImpl(); }

  emp::optional<bool> HoldsThreadImpl() const {
    return inlet.HoldsThreadImpl();
  }

  emp::optional<bool> HoldsProcImpl() const { return inlet.HoldsProcImpl(); }

};

} // namespace internal
} // namesapce uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_INLET_CACHINGINLETWRAPPER_HPP_INCLUDE
