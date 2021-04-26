#pragma once
#ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_SKIPPING_TYPE_SPAN_INLET_RINGIRSEND_OUTLET_BLOCKIRECV_S__IRIROBIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_DROPPING_GET_SKIPPING_TYPE_SPAN_INLET_RINGIRSEND_OUTLET_BLOCKIRECV_S__IRIROBIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/put=dropping+type=span/s::RingIrsendDuct.hpp"
#include "../impl/outlet/get=skipping+type=span/s::BlockIrecvDuct.hpp"

namespace uit {
namespace s {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IrirObiDuct {

  using InletImpl = uit::s::RingIrsendDuct<ImplSpec>;
  using OutletImpl = uit::s::BlockIrecvDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace s
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_SKIPPING_TYPE_SPAN_INLET_RINGIRSEND_OUTLET_BLOCKIRECV_S__IRIROBIDUCT_HPP_INCLUDE
