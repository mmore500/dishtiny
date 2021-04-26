#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_T__RSENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_T__RSENDDUCT_HPP_INCLUDE

#include "../../../../../../uitsl/mpi/routine_functors.hpp"

#include "impl/TrivialBlockingSendDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RsendDuct
: public uit::internal::TrivialBlockingSendDuct<
  uitsl::RsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::TrivialBlockingSendDuct<
    uitsl::RsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "RsendDuct"; }

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_T__RSENDDUCT_HPP_INCLUDE
