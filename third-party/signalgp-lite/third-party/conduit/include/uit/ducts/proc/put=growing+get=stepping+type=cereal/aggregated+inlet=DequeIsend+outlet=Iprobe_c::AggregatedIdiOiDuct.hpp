#pragma once
#ifndef UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_AGGREGATED_INLET_DEQUEISEND_OUTLET_IPROBE_C__AGGREGATEDIDIOIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_AGGREGATED_INLET_DEQUEISEND_OUTLET_IPROBE_C__AGGREGATEDIDIOIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/templated/AggregatedInletDuct.hpp"
#include "../impl/outlet/templated/AggregatedOutletDuct.hpp"

#include "inlet=DequeIsend+outlet=Iprobe_c::IdiOiDuct.hpp"

namespace uit {
namespace c {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class AggregatedIdiOiDuct {

  template<typename Spec>
  using BackingDuct = uit::c::IdiOiDuct<Spec>;

public:

  using InletImpl = uit::AggregatedInletDuct<BackingDuct, ImplSpec>;
  using OutletImpl = uit::AggregatedOutletDuct<BackingDuct, ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace c
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_AGGREGATED_INLET_DEQUEISEND_OUTLET_IPROBE_C__AGGREGATEDIDIOIDUCT_HPP_INCLUDE
