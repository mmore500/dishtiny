#pragma once
#ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_TRIVIAL_BUFFERED_INLET_RINGISEND_OUTLET_IPROBE_T__BUFFEREDIRIOIDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_TRIVIAL_BUFFERED_INLET_RINGISEND_OUTLET_IPROBE_T__BUFFEREDIRIOIDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/templated/BufferedInletDuct.hpp"
#include "../impl/outlet/templated/BufferedOutletDuct.hpp"

#include "../put=dropping+get=stepping+type=span/inlet=RingIsend+outlet=Iprobe_s::IriOiDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class BufferedIriOiDuct {

  template<typename Spec>
  using BackingDuct = uit::s::IriOiDuct<Spec>;

public:

  using InletImpl = uit::BufferedInletDuct<BackingDuct, ImplSpec>;
  using OutletImpl = uit::BufferedOutletDuct<BackingDuct, ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_PUT_DROPPING_GET_STEPPING_TYPE_TRIVIAL_BUFFERED_INLET_RINGISEND_OUTLET_IPROBE_T__BUFFEREDIRIOIDUCT_HPP_INCLUDE
