#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_ISPARENTGROUPOF_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_ISPARENTGROUPOF_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

#include "../../../../config/cfg.hpp"

namespace dish2 {

/**
 * Did this cell's kin group descend directly from the cardinal's neighbor
 * cell's kin group?
 *
 * That is, is cell's kin group ancestor ID(s) equal to the cardinal's
 * neighbor's current kin group ID(s).
 */
template< typename Spec >
struct IsParentGroupOf : public uitsl::PodLeafNode< char, Spec::NLEV > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<char, Spec::NLEV>;
  using parent_t::parent_t;

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME_TEMPLATE( dish2::IsParentGroupOf );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_ISPARENTGROUPOF_HPP_INCLUDE
