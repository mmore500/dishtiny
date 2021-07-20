#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_PHYLOGENETICROOTVIEW_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_PHYLOGENETICROOTVIEW_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../../../../config/cfg.hpp"

namespace dish2 {

/**
 * What is this cell's phylogenetic root ID?
 *
 * (Which initially-generated ancestor is this cell descended from?)
 */
struct PhylogeneticRootView : public uitsl::PodLeafNode< size_t > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<size_t>;
  using parent_t::parent_t;

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME( dish2::PhylogeneticRootView );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_PHYLOGENETICROOTVIEW_HPP_INCLUDE
