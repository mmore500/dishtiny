#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_NEIGHBORFRAGMENTED_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_NEIGHBORFRAGMENTED_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

#include "../../../../config/cfg.hpp"

namespace dish2 {

/**
 * Was group fragmentation the most recent cause of death in the cardinal's
 * neighbor cell?
 */
struct NeighborFragmented : public uitsl::PodLeafNode< char > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<char>;
  using parent_t::parent_t;

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::NeighborFragmented );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_NEIGHBORFRAGMENTED_HPP_INCLUDE
