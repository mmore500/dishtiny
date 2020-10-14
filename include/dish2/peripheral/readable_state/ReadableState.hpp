#pragma once
#ifndef DISH__PERIPHERAL_READABLESTATE_READABLESTATE_HPP_INCLUDE
#define DISH__PERIPHERAL_READABLESTATE_READABLESTATE_HPP_INCLUDE

#include "../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"

#include "IntrospectiveState.hpp"

namespace dish2 {

struct ReadableState
: public uitsl::PodInternalNode< uitsl::PodLeafNode<dish2::IntrospectiveState> >
{};

} // namespace dish2

#endif // #ifndef DISH__PERIPHERAL_READABLESTATE_READABLESTATE_HPP_INCLUDE
