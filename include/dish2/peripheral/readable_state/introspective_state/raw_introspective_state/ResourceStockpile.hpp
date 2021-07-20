#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RESOURCESTOCKPILE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RESOURCESTOCKPILE_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

namespace dish2 {

/**
 * Amount of resource this cell has.
 *
 * @note The canonical amount of resource the cell posesses is stored here.
 * It is assumed that this value is kept in sync across cardinals.
 * (This isn't a view of the amount resource posessed, it is the implementation
 * of resource tracking.)
 */
struct ResourceStockpile : public uitsl::PodLeafNode<float> {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<float>;
  using parent_t::parent_t;
};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::ResourceStockpile );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_RESOURCESTOCKPILE_HPP_INCLUDE
