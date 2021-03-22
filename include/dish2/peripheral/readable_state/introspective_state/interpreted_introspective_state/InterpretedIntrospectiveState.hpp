#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_INTERPRETEDINTROSPECTIVESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_INTERPRETEDINTROSPECTIVESTATE_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/signalgp-lite/include/sgpl/utility/ByteEnumeration.hpp"

namespace dish2 {

namespace internal {

template< typename Spec >
using interpreted_introspective_state_parent_t = uitsl::PodInternalNode<
  >;

} // namespace internal


template<typename Spec>
struct InterpretedIntrospectiveState
: public internal::interpreted_introspective_state_parent_t<Spec>{

  using parent_t = internal::interpreted_introspective_state_parent_t<Spec>;
  constexpr inline static size_t parent_size = parent_t::GetSize();

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTERPRETED_INTROSPECTIVE_STATE_INTERPRETEDINTROSPECTIVESTATE_HPP_INCLUDE
