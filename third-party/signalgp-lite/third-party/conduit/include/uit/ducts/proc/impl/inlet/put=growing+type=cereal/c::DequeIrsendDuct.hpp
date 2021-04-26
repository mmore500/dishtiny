#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_CEREAL_C__DEQUEIRSENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_CEREAL_C__DEQUEIRSENDDUCT_HPP_INCLUDE

#include "../../../../../../uitsl/mpi/routine_functors.hpp"

#include "impl/CerealDequeImmediateSendDuct.hpp"

namespace uit {
namespace c {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class DequeIrsendDuct
: public uit::internal::CerealDequeImmediateSendDuct<
  uitsl::IrsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::CerealDequeImmediateSendDuct<
    uitsl::IrsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "DequeIrsendDuct"; }

};

} // namespace c
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_CEREAL_C__DEQUEIRSENDDUCT_HPP_INCLUDE
