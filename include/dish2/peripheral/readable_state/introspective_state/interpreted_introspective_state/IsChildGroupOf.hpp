#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_ISCHILDGROUPOF_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_ISCHILDGROUPOF_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

#include "../../../../config/cfg.hpp"

namespace dish2 {

/**
 * Does this cell's kin group ID descend directly from the neighbor's kin group
 * ID?
 */
template< typename Spec >
struct IsChildGroupOf : public uitsl::PodLeafNode< char, Spec::NLEV > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<char, Spec::NLEV>;
  using parent_t::parent_t;
};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME_TEMPLATE( dish2::IsChildGroupOf );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_ISCHILDGROUPOF_HPP_INCLUDE
