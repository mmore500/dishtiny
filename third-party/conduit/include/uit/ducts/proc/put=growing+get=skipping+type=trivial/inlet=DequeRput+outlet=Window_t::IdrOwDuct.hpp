#pragma once
#ifndef UIT_DUCTS_PROC_PUT_GROWING_GET_SKIPPING_TYPE_TRIVIAL_INLET_DEQUERPUT_OUTLET_WINDOW_T__IDROWDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_GROWING_GET_SKIPPING_TYPE_TRIVIAL_INLET_DEQUERPUT_OUTLET_WINDOW_T__IDROWDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/put=growing+type=trivial/t::DequeRputDuct.hpp"
#include "../impl/outlet/get=skipping+type=trivial/t::WindowDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IdrOwDuct {

  using InletImpl = uit::t::DequeRputDuct<ImplSpec>;
  using OutletImpl = uit::t::WindowDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_GROWING_GET_SKIPPING_TYPE_TRIVIAL_INLET_DEQUERPUT_OUTLET_WINDOW_T__IDROWDUCT_HPP_INCLUDE
