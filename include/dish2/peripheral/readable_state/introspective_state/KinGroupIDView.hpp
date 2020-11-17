#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_KINGROUPIDVIEW_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_KINGROUPIDVIEW_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

namespace dish2 {

template< typename Spec >
struct KinGroupIDView : public uitsl::PodLeafNode< size_t, Spec::NLEV > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<size_t, Spec::NLEV>;
  using parent_t::parent_t;
};

} // namespace dish2

namespace uitsl {

// UITSL_ENABLE_TYPENAME( dish2::KinGroupIDView );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_KINGROUPIDVIEW_HPP_INCLUDE
