#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_SPAN_S__RINGISENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_SPAN_S__RINGISENDDUCT_HPP_INCLUDE

#include "../../../../../../uitsl/mpi/routine_functors.hpp"

#include "impl/SpanRingImmediateSendDuct.hpp"

namespace uit {
namespace s {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RingIsendDuct
: public uit::internal::SpanRingImmediateSendDuct<
  uitsl::IsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::SpanRingImmediateSendDuct<
    uitsl::IsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;


  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "RingIsendDuct"; }

};

} // namespace s
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_SPAN_S__RINGISENDDUCT_HPP_INCLUDE
