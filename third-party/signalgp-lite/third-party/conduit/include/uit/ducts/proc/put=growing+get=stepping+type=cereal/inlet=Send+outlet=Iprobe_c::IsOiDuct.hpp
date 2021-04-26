#pragma once
#ifndef UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_INLET_SEND_OUTLET_IPROBE_C__ISOIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_INLET_SEND_OUTLET_IPROBE_C__ISOIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/put=growing+type=cereal/c::SendDuct.hpp"
#include "../impl/outlet/get=stepping+type=cereal/c::IprobeDuct.hpp"

namespace uit {
namespace c {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IsOiDuct {

  using InletImpl = uit::c::SendDuct<ImplSpec>;
  using OutletImpl = uit::c::IprobeDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace c
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_INLET_SEND_OUTLET_IPROBE_C__ISOIDUCT_HPP_INCLUDE
