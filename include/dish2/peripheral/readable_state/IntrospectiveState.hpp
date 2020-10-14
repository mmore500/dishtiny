#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE

#include "../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"

namespace dish2 {

struct ResourceStockpile : public uitsl::PodLeafNode<double> {};

struct IntrospectiveState
: public uitsl::PodInternalNode< dish2::ResourceStockpile >
{};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE
