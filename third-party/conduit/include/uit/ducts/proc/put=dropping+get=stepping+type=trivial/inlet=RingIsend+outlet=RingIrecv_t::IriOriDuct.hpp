#pragma once
#ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_TRIVIAL_INLET_RINGISEND_OUTLET_RINGIRECV_T__IRIORIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_TRIVIAL_INLET_RINGISEND_OUTLET_RINGIRECV_T__IRIORIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/put=dropping+type=trivial/t::RingIsendDuct.hpp"
#include "../impl/outlet/get=stepping+type=trivial/t::RingIrecvDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IriOriDuct {

  using InletImpl = uit::t::RingIsendDuct<ImplSpec>;
  using OutletImpl = uit::t::RingIrecvDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_TRIVIAL_INLET_RINGISEND_OUTLET_RINGIRECV_T__IRIORIDUCT_HPP_INCLUDE
