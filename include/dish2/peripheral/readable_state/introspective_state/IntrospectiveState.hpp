#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

#include "CellAge.hpp"
#include "Epoch.hpp"
#include "IncomingInterMessageCounter.hpp"
#include "IncomingIntraMessageCounter.hpp"
#include "IsAlive.hpp"
#include "KinGroupAge.hpp"
#include "KinGroupIDView.hpp"
#include "ResourceStockpile.hpp"

namespace dish2 {

template<typename Spec>
struct IntrospectiveState
: public uitsl::PodInternalNode<
  dish2::CellAge,
  dish2::Epoch,
  dish2::IncomingInterMessageCounter,
  dish2::IncomingIntraMessageCounter,
  dish2::IsAlive,
  dish2::KinGroupAge<Spec>,
  dish2::KinGroupIDView<Spec>,
  dish2::ResourceStockpile
>
{};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE
