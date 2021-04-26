#pragma once
#ifndef UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_INLET_DEQUEISEND_OUTLET_IPROBE_C__IDIOIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_INLET_DEQUEISEND_OUTLET_IPROBE_C__IDIOIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/put=growing+type=cereal/c::DequeIsendDuct.hpp"
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
struct IdiOiDuct {

  using InletImpl = uit::c::DequeIsendDuct<ImplSpec>;
  using OutletImpl = uit::c::IprobeDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace c
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_GROWING_GET_STEPPING_TYPE_CEREAL_INLET_DEQUEISEND_OUTLET_IPROBE_C__IDIOIDUCT_HPP_INCLUDE
