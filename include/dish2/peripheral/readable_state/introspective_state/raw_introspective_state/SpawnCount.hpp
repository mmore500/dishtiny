#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_SPAWNCOUNT_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_SPAWNCOUNT_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

namespace dish2 {

struct SpawnCount : public uitsl::PodLeafNode<char> {
  using parent_t = uitsl::PodLeafNode<char>;
  using parent_t::parent_t;
};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::SpawnCount );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_SPAWNCOUNT_HPP_INCLUDE
