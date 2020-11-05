#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"

#include "ApoptosisRequest.hpp"
#include "RepLevRequest.hpp"
#include "ResourceReceiveResistance.hpp"
#include "ResourceReserveRequest.hpp"
#include "ResourceSendLimit.hpp"
#include "ResourceSendRequest.hpp"
#include "SpawnArrest.hpp"
#include "SpawnRequest.hpp"

namespace dish2 {

template< typename Spec >
struct ControllerMappedState
: public uitsl::PodInternalNode<
  dish2::ApoptosisRequest,
  dish2::RepLevRequest< Spec >,
  dish2::ResourceReceiveResistance,
  dish2::ResourceReserveRequest,
  dish2::ResourceSendLimit,
  dish2::ResourceSendRequest,
  dish2::SpawnArrest,
  dish2::SpawnRequest
>
{};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE
