#pragma once
#ifndef UIT_DUCTS_PROC_ACCUMULATING_TYPE_TRIVIAL_AGGREGATED_INLET_ISEND_OUTLET_IRECV_T__AGGREGATEDIIOIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_ACCUMULATING_TYPE_TRIVIAL_AGGREGATED_INLET_ISEND_OUTLET_IRECV_T__AGGREGATEDIIOIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../accumulating+type=span/inlet=Isend+outlet=Irecv_s::IiOiDuct.hpp"
#include "../impl/inlet/templated/AccumulatingPooledInletDuct.hpp"
#include "../impl/outlet/templated/AccumulatingPooledOutletDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class AggregatedIiOiDuct {

  template<typename Spec>
  using BackingDuct = uit::s::IiOiDuct<Spec>;

public:

  using InletImpl = uit::AccumulatingPooledInletDuct<BackingDuct, ImplSpec>;
  using OutletImpl = uit::AccumulatingPooledOutletDuct<BackingDuct, ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_ACCUMULATING_TYPE_TRIVIAL_AGGREGATED_INLET_ISEND_OUTLET_IRECV_T__AGGREGATEDIIOIDUCT_HPP_INCLUDE
