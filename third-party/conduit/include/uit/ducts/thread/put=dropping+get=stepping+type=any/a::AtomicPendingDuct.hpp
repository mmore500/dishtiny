#pragma once
#ifndef UIT_DUCTS_THREAD_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__ATOMICPENDINGDUCT_HPP_INCLUDE
#define UIT_DUCTS_THREAD_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__ATOMICPENDINGDUCT_HPP_INCLUDE

#include <atomic>
#include <string>

#include "../../../../uitsl/parallel/AlignedImplicit.hpp"
#include "../../../../uitsl/parallel/AlignedInherit.hpp"

#include "../../intra/put=dropping+get=stepping+type=any/impl/PendingDuct.hpp"


namespace uit {
namespace a {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class AtomicPendingDuct
: public uit::internal::PendingDuct<
  uitsl::AlignedInherit<std::atomic<size_t>>,
  uitsl::AlignedImplicit<typename ImplSpec::T>,
  ImplSpec
>
{

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "AtomicPendingDuct"; }

};

} // namespace a
} // namespace uit

#endif // #ifndef UIT_DUCTS_THREAD_PUT_DROPPING_GET_STEPPING_TYPE_ANY_A__ATOMICPENDINGDUCT_HPP_INCLUDE
