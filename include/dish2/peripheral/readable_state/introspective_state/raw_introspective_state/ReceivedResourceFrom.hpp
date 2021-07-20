#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RECEIVEDRESOURCEFROM_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RECEIVEDRESOURCEFROM_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../../../../config/cfg.hpp"

namespace dish2 {

/**
 * How much resource is being received from the cardinal's cell neighbor?
 */
struct ReceivedResourceFrom : public uitsl::PodLeafNode< float > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<float>;
  using parent_t::parent_t;

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::ReceivedResourceFrom );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RECEIVEDRESOURCEFROM_HPP_INCLUDE
