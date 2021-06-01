#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_NUMKNOWNQUORUMBITS_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_NUMKNOWNQUORUMBITS_HPP_INCLUDE

#include <algorithm>

#include "../../../../../../third-party/conduit/include/uitsl/algorithm/count_equal.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace dish2 {

template< typename Spec >
struct NumKnownQuorumBits : public uitsl::PodLeafNode< size_t, Spec::NLEV > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<size_t, Spec::NLEV>;
  using parent_t::parent_t;

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME_TEMPLATE( dish2::NumKnownQuorumBits );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_NUMKNOWNQUORUMBITS_HPP_INCLUDE
