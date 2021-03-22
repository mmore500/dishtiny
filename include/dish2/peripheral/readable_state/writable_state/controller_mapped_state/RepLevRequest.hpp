#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_REPLEVREQUEST_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_REPLEVREQUEST_HPP_INCLUDE

#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

namespace dish2 {

template< typename Spec >
struct RepLevRequest : public uitsl::PodLeafNode<char, Spec::NLEV> {
  // inherit constructors
  using parent_t = uitsl::PodLeafNode<char, Spec::NLEV>;
  using parent_t::parent_t;

  size_t GetRepLev() const {

    const size_t count = std::count_if(
      std::begin( parent_t::GetBuffer() ),
      std::end( parent_t::GetBuffer() ),
      std::identity
    );
    if ( count == 0 ) return Spec::NLEV;
    else return count - 1;
  }
};

} // namespace dish2

namespace uitsl {

UITSL_ENABLE_TYPENAME_TEMPLATE( dish2::RepLevRequest );

} // namespace uitsl

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_REPLEVREQUEST_HPP_INCLUDE
