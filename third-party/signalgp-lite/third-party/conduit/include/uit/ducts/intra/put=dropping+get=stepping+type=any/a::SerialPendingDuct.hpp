#pragma once
#ifndef UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__SERIALPENDINGDUCT_HPP_INCLUDE
#define UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__SERIALPENDINGDUCT_HPP_INCLUDE

#include "impl/PendingDuct.hpp"

namespace uit {
namespace a {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class SerialPendingDuct
: public uit::internal::PendingDuct<
  size_t,
  typename ImplSpec::T,
  ImplSpec
> {

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "SerialPendingDuct"; }

};

} // namespace a
} // namespace uit

#endif // #ifndef UIT_DUCTS_INTRA_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__SERIALPENDINGDUCT_HPP_INCLUDE
