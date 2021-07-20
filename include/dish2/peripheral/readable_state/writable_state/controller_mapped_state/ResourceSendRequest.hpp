#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_RESOURCESENDREQUEST_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_RESOURCESENDREQUEST_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

namespace dish2 {

/**
 * Setting this state initiates resource sharing to the cardinal's neighbor cell.
 *
 * The value stored controls the amount of resource shared.
 */
struct ResourceSendRequest : public uitsl::PodLeafNode<float> {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<float>;
  using parent_t::parent_t;
};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::ResourceSendRequest );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_RESOURCESENDREQUEST_HPP_INCLUDE
