#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPIDVIEW_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPIDVIEW_HPP_INCLUDE

#include <algorithm>

#include "../../../../../../third-party/conduit/include/uitsl/algorithm/count_equal.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace dish2 {

/**
 * Kin group ID of this cell.
 */
template< typename Spec >
struct KinGroupIDView : public uitsl::PodLeafNode< uint64_t, Spec::NLEV > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<uint64_t, Spec::NLEV>;
  using parent_t::parent_t;

  size_t CountCommonality( const KinGroupIDView& other ) const {
    const auto& buffer = parent_t::GetBuffer();
    const auto& other_buffer = other.GetBuffer();
    const auto mismatch = std::mismatch(
      std::rbegin( buffer ), std::rend( buffer ),
      std::rbegin( other_buffer )
    ).first;
    const size_t res = uitsl::audit_cast<size_t>(
      std::distance( std::rbegin(buffer), mismatch )
    );
    emp_assert( res == uitsl::count_equal(
      std::begin(buffer), std::end(buffer), std::begin(other_buffer)
    ) );
    return res;
  }

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME_TEMPLATE( dish2::KinGroupIDView );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPIDVIEW_HPP_INCLUDE
