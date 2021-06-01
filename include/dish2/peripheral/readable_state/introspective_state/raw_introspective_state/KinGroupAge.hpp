#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPAGE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPAGE_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"

#include "../../../../genome/KinGroupEpochStamps.hpp"

namespace dish2 {

template< typename Spec >
struct KinGroupAge : public uitsl::PodLeafNode< uint64_t, Spec::NLEV > {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<uint64_t, Spec::NLEV>;
  using parent_t::parent_t;

  void Refresh(
    const uint64_t epoch, const dish2::KinGroupEpochStamps< Spec > stamps
  ) {
    std::transform(
      std::begin( stamps.GetBuffer() ),
      std::end( stamps.GetBuffer() ),
      std::begin( *this ),
      [epoch]( const auto stamp ){ return epoch - stamp; }
    );
  }

};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME_TEMPLATE( dish2::KinGroupAge );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_RAW_INTROSPECTIVE_STATE_KINGROUPAGE_HPP_INCLUDE
