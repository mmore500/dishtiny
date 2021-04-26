#pragma once
#ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_CEREAL_AGGREGATED_INLET_RINGISEND_OUTLET_IPROBE_C__AGGREGATEDIRIOIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_CEREAL_AGGREGATED_INLET_RINGISEND_OUTLET_IPROBE_C__AGGREGATEDIRIOIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/templated/AggregatedInletDuct.hpp"
#include "../impl/outlet/templated/AggregatedOutletDuct.hpp"

#include "inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"

namespace uit {
namespace c {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class AggregatedIriOiDuct {

  template<typename Spec>
  using BackingDuct = uit::c::IriOiDuct<Spec>;

public:

  using InletImpl = uit::AggregatedInletDuct<BackingDuct, ImplSpec>;
  using OutletImpl = uit::AggregatedOutletDuct<BackingDuct, ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_CEREAL_AGGREGATED_INLET_RINGISEND_OUTLET_IPROBE_C__AGGREGATEDIRIOIDUCT_HPP_INCLUDE
