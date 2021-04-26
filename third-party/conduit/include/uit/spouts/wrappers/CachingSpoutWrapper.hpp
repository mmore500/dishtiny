#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_CACHINGSPOUTWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_CACHINGSPOUTWRAPPER_HPP_INCLUDE

#include "../../../uitsl/distributed/CachePacket.hpp"
#include "../../../uitsl/meta/IdentityWrapper.hpp"

#include "inlet/CachingInletWrapper.hpp"
#include "outlet/CachingOutletWrapper.hpp"

namespace uit {

template<typename T_>
class CachingSpoutWrapper {

public:
  using T = uitsl::CachePacket<T_>;

  template<typename Inlet>
  using inlet_wrapper_t = uit::internal::CachingInletWrapper<Inlet>;

  template<typename Outlet>
  using outlet_wrapper_t = uit::internal::CachingOutletWrapper<Outlet>;

};



} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_CACHINGSPOUTWRAPPER_HPP_INCLUDE
