#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"

namespace dish2 {

struct SharingRequest : public uitsl::PodLeafNode<double> {
};

struct SharingResistance : public uitsl::PodLeafNode<double> {
};

struct ControllerMappedState
: public uitsl::PodInternalNode<
  dish2::SharingRequest,
  dish2::SharingResistance
>
{};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE
