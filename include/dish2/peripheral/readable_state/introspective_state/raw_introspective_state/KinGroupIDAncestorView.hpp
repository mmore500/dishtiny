#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPIDANCESTORVIEW_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPIDANCESTORVIEW_HPP_INCLUDE

#include <algorithm>

#include "../../../../../../third-party/conduit/include/uitsl/algorithm/count_equal.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace dish2 {

/**
 * Kin group ID from which cell's kin group ID is descended.
 */
template< typename Spec >
struct KinGroupIDAncestorView : public uitsl::PodLeafNode< size_t, Spec::NLEV > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<size_t, Spec::NLEV>;
  using parent_t::parent_t;

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME_TEMPLATE( dish2::KinGroupIDAncestorView );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPIDANCESTORVIEW_HPP_INCLUDE
