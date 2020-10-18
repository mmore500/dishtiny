#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"

#include "IsAlive.hpp"
#include "ResourceStockpile.hpp"

namespace dish2 {

struct IntrospectiveState
: public uitsl::PodInternalNode<
  dish2::IsAlive,
  dish2::ResourceStockpile
>
{};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE
