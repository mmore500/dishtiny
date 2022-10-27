#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_MOSTRECENTCAUSEOFDEATH_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_MOSTRECENTCAUSEOFDEATH_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

namespace dish2 {

struct MostRecentCauseOfDeath : public uitsl::PodLeafNode<char> {
  using parent_t = uitsl::PodLeafNode<char>;
  using parent_t::parent_t;
};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::MostRecentCauseOfDeath );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_MOSTRECENTCAUSEOFDEATH_HPP_INCLUDE
