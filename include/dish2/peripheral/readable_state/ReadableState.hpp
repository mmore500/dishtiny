#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_READABLESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_READABLESTATE_HPP_INCLUDE

#include "../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../third-party/Empirical/source/base/macros.h"
#include "../../../../third-party/signalgp-lite/include/sgpl/utility/ByteEnumeration.hpp"

#include "introspective_state/IntrospectiveState.hpp"
#include "writable_state/WritableState.hpp"

namespace dish2 {

namespace internal {

  template<typename Spec>
  using readable_state_parent_t = uitsl::PodInternalNode<
    dish2::IntrospectiveState<Spec>,
    dish2::WritableState<Spec>
  >;

}

template<typename Spec>
struct ReadableState : public internal::readable_state_parent_t<Spec> {

  using parent_t = internal::readable_state_parent_t<Spec>;

  // https://stackoverflow.com/a/63046442
  template<size_t Templateify=0>
  double Read(const size_t idx) const {

    #define DISH2_READABLE_STATE_CASE_PAYLOAD(N) \
      case N: \
        if constexpr (N < parent_t::GetSize()){ \
          return this->template Get<N + Templateify>(); \
        } \
      break;

    emp_assert( idx < parent_t::GetSize() );
    static_assert( parent_t::GetSize() < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH( DISH2_READABLE_STATE_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

    }

    emp_assert( false );
    return 0;

  }

  static std::string GetLeafTypeName( const size_t idx ) {

    #define DISH2_READABLE_STATE_NAME_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_t::GetSize() ) { \
          return uitsl::TypeName< \
            typename parent_t::template leaf_t<N> \
          >::Get(); \
        }

    emp_assert( idx < parent_t::GetSize() );
    static_assert( parent_t::GetSize() < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_READABLE_STATE_NAME_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

    }

    return "bad idx";

  }

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_READABLESTATE_HPP_INCLUDE
