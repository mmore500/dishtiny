#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_ISALIVE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_ISALIVE_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"

namespace dish2 {

struct IsAlive : public uitsl::PodLeafNode<char> {
  using parent_t = uitsl::PodLeafNode<char>;
  using parent_t::parent_t;
};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_ISALIVE_HPP_INCLUDE
