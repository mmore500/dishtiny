#pragma once
#ifndef DISH__PERIPHERAL_READABLESTATE_WRITABLESTATE_WRITABLESTATE_HPP_INCLUDE
#define DISH__PERIPHERAL_READABLESTATE_WRITABLESTATE_WRITABLESTATE_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"

#include "ControllerMappedState.hpp"

namespace dish2 {

struct WritableState
: public uitsl::PodInternalNode< dish2::ControllerMappedState >
{};

} // namespace dish2

#endif // #ifndef DISH__PERIPHERAL_READABLESTATE_WRITABLESTATE_WRITABLESTATE_HPP_INCLUDE
