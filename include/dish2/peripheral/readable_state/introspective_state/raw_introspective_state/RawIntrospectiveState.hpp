#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RAWINTROSPECTIVESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RAWINTROSPECTIVESTATE_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/signalgp-lite/include/sgpl/utility/ByteEnumeration.hpp"

#include "CellAge.hpp"
#include "Epoch.hpp"
#include "IncomingInterMessageCounter.hpp"
#include "IncomingIntraMessageCounter.hpp"
#include "IsAlive.hpp"
#include "KinGroupAge.hpp"
#include "KinGroupIDAncestorView.hpp"
#include "KinGroupIDView.hpp"
#include "MostRecentCauseOfDeath.hpp"
#include "NumKnownQuorumBits.hpp"
#include "PhylogeneticRootView.hpp"
#include "ReceivedResourceFrom.hpp"
#include "ResourceStockpile.hpp"
#include "SpawnCount.hpp"
#include "SpawnedFrom.hpp"

namespace dish2 {

namespace internal {

template< typename Spec >
using raw_introspective_state_parent_t = uitsl::PodInternalNode<
  dish2::CellAge,
  dish2::Epoch,
  dish2::IncomingInterMessageCounter,
  dish2::IncomingIntraMessageCounter,
  dish2::IsAlive,
  dish2::KinGroupAge<Spec>,
  dish2::KinGroupIDAncestorView<Spec>,
  dish2::KinGroupIDView<Spec>,
  dish2::MostRecentCauseOfDeath,
  dish2::NumKnownQuorumBits<Spec>,
  dish2::PhylogeneticRootView,
  dish2::ReceivedResourceFrom,
  dish2::ResourceStockpile,
  dish2::SpawnCount,
  dish2::SpawnedFrom
  >;

} // namespace internal


/**
 * Raw introspective state directly exposes aspects of simulation state.
 */
template<typename Spec>
struct RawIntrospectiveState
: public internal::raw_introspective_state_parent_t<Spec>{

  using parent_t = internal::raw_introspective_state_parent_t<Spec>;
  constexpr inline static size_t parent_size = parent_t::GetSize();

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RAWINTROSPECTIVESTATE_HPP_INCLUDE
