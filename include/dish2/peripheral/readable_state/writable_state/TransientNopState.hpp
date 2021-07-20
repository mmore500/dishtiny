#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_TRANSIENTNOPSTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_TRANSIENTNOPSTATE_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

namespace dish2 {


/**
 * Writing to this state has no external effect.
 *
 * It is cleared regularly by `dish2::DecayToBaselineService`. It can be used
 * as temporary global memory shared between cores of a cardinal's virtual CPU.
 */
template< typename Spec >
struct TransientNopState
: public uitsl::PodLeafNode<float, Spec::AMT_NOP_MEMORY> {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<float, Spec::AMT_NOP_MEMORY>;
  using parent_t::parent_t;
};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME_TEMPLATE( dish2::TransientNopState );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_TRANSIENTNOPSTATE_HPP_INCLUDE
