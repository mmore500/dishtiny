#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_NEIGHBORAPOPTOSIS_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_NEIGHBORAPOPTOSIS_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

#include "../../../../config/cfg.hpp"

namespace dish2 {

/**
 * Was the neighbor tile's most recent death apoptosis?
 */
struct NeighborApoptosis : public uitsl::PodLeafNode< char > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<char>;
  using parent_t::parent_t;

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::NeighborApoptosis );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_NEIGHBORAPOPTOSIS_HPP_INCLUDE
