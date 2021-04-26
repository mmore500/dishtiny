#pragma once
#ifndef UIT_DUCTS_PROC_ACCUMULATING_TYPE_SPANFUNDAMENTAL_INLET_RACCUMULATE_OUTLET_WITHDRAWINGWINDOW_SF__IROWWDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_ACCUMULATING_TYPE_SPANFUNDAMENTAL_INLET_RACCUMULATE_OUTLET_WITHDRAWINGWINDOW_SF__IROWWDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/accumulating+type=spanfundamental/sf::RaccumulateDuct.hpp"
#include "../impl/outlet/accumulating+type=spanfundamental/sf::WithdrawingWindowDuct.hpp"

namespace uit {
namespace sf {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IrOwwDuct {

  using InletImpl = uit::sf::RaccumulateDuct<ImplSpec>;
  using OutletImpl = uit::sf::WithdrawingWindowDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace sf
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_ACCUMULATING_TYPE_SPANFUNDAMENTAL_INLET_RACCUMULATE_OUTLET_WITHDRAWINGWINDOW_SF__IROWWDUCT_HPP_INCLUDE
