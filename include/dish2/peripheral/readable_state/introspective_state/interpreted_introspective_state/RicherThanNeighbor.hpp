#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_RICHERTHANNEIGHBOR_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_RICHERTHANNEIGHBOR_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

#include "../../../../config/cfg.hpp"

namespace dish2 {

/**
 * Does this cell's stockpile more resource than the cardinal's cell neighbor?
 */
struct RicherThanNeighbor : public uitsl::PodLeafNode< char > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<char>;
  using parent_t::parent_t;

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::RicherThanNeighbor );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_RICHERTHANNEIGHBOR_HPP_INCLUDE
