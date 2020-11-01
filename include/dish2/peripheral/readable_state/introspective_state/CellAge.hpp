#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_CELLAGE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_CELLAGE_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"

namespace dish2 {

struct CellAge : public uitsl::PodLeafNode<size_t> {
  using parent_t = uitsl::PodLeafNode<size_t>;
  using parent_t::parent_t;
};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_CELLAGE_HPP_INCLUDE
