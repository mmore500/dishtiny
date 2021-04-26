#pragma once
#ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_SKIPPING_TYPE_TRIVIAL_POOLED_INLET_RINGISEND_OUTLET_BLOCKIRECV_T__POOLEDIRIOBIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_DROPPING_GET_SKIPPING_TYPE_TRIVIAL_POOLED_INLET_RINGISEND_OUTLET_BLOCKIRECV_T__POOLEDIRIOBIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/templated/PooledInletDuct.hpp"
#include "../impl/outlet/templated/PooledOutletDuct.hpp"

#include "../put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlockIrecv_s::IriObiDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class PooledIriObiDuct {

  template<typename Spec>
  using BackingDuct = uit::s::IriObiDuct<Spec>;

public:

  using InletImpl = uit::PooledInletDuct<BackingDuct, ImplSpec>;
  using OutletImpl = uit::PooledOutletDuct<BackingDuct, ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_SKIPPING_TYPE_TRIVIAL_POOLED_INLET_RINGISEND_OUTLET_BLOCKIRECV_T__POOLEDIRIOBIDUCT_HPP_INCLUDE
