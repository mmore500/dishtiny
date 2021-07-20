#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_INTERPRETEDINTROSPECTIVESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_INTERPRETEDINTROSPECTIVESTATE_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/signalgp-lite/include/sgpl/utility/ByteEnumeration.hpp"

#include "IsChildCellOf.hpp"
#include "IsChildGroupOf.hpp"
#include "IsNewborn.hpp"
#include "IsParentCellOf.hpp"
#include "IsParentGroupOf.hpp"
#include "KinGroupMatch.hpp"
#include "KinGroupWillExpire.hpp"
#include "NeighborApoptosis.hpp"
#include "NeighborFragmented.hpp"
#include "NeighborIsNewborn.hpp"
#include "NeighborKinGroupWillExpire.hpp"
#include "NeighborOptimumQuorumExceeded.hpp"
#include "OptimumQuorumExceeded.hpp"
#include "ParentFragmented.hpp"
#include "PhylogeneticRootMatch.hpp"
#include "RicherThanNeighbor.hpp"
#include "StockpileDepleted.hpp"
#include "StockpileFecund.hpp"

namespace dish2 {

namespace internal {

template< typename Spec >
using interpreted_introspective_state_parent_t = uitsl::PodInternalNode<
  dish2::IsChildCellOf,
  dish2::IsChildGroupOf<Spec>,
  dish2::IsNewborn,
  dish2::IsParentCellOf,
  dish2::IsParentGroupOf<Spec>,
  dish2::KinGroupMatch<Spec>,
  dish2::KinGroupWillExpire<Spec>,
  dish2::NeighborApoptosis,
  dish2::NeighborFragmented,
  dish2::NeighborIsNewborn,
  dish2::NeighborKinGroupWillExpire<Spec>,
  dish2::NeighborOptimumQuorumExceeded<Spec>,
  dish2::OptimumQuorumExceeded<Spec>,
  dish2::ParentFragmented,
  dish2::PhylogeneticRootMatch,
  dish2::RicherThanNeighbor,
  dish2::StockpileDepleted,
  dish2::StockpileFecund
  >;

} // namespace internal

/**
 * Interpreted introspective state is filled with truthy values that are
 * interpreted as booleans to dispatch environmentally-managed events.
 */
template<typename Spec>
struct InterpretedIntrospectiveState
: public internal::interpreted_introspective_state_parent_t<Spec>{

  using parent_t = internal::interpreted_introspective_state_parent_t<Spec>;
  constexpr inline static size_t parent_size = parent_t::GetSize();

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_INTERPRETEDINTROSPECTIVESTATE_HPP_INCLUDE
