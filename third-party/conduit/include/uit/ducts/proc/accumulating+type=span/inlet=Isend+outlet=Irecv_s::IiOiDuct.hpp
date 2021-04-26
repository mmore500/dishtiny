#pragma once
#ifndef UIT_DUCTS_PROC_ACCUMULATING_TYPE_SPAN_INLET_ISEND_OUTLET_IRECV_S__IIOIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_ACCUMULATING_TYPE_SPAN_INLET_ISEND_OUTLET_IRECV_S__IIOIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/accumulating+type=span/s::IsendDuct.hpp"
#include "../impl/outlet/accumulating+type=span/s::IrecvDuct.hpp"

namespace uit {
namespace s {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IiOiDuct {

  using InletImpl = uit::s::IsendDuct<ImplSpec>;
  using OutletImpl = uit::s::IrecvDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace s
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_ACCUMULATING_TYPE_SPAN_INLET_ISEND_OUTLET_IRECV_S__IIOIDUCT_HPP_INCLUDE
