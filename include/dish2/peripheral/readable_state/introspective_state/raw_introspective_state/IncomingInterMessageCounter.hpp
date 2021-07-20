#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_INCOMINGINTERMESSAGECOUNTER_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_INCOMINGINTERMESSAGECOUNTER_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

namespace dish2 {

/**
 * Counter tracking incoming messages from cardinal's neighbor cell.
 *
 * Intermittently reset to zero.
 */
struct IncomingInterMessageCounter : public uitsl::PodLeafNode<size_t> {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<size_t>;
  using parent_t::parent_t;
};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::IncomingInterMessageCounter );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_INCOMINGINTERMESSAGECOUNTER_HPP_INCLUDE
