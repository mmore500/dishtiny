#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../third-party/conduit/include/uitsl/meta/TypeName.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/signalgp-lite/include/sgpl/utility/ByteEnumeration.hpp"

#include "interpreted_introspective_state/InterpretedIntrospectiveState.hpp"
#include "raw_introspective_state/RawIntrospectiveState.hpp"

namespace dish2 {

namespace internal {

template< typename Spec >
using introspective_state_parent_t = uitsl::PodInternalNode<
  dish2::InterpretedIntrospectiveState<Spec>,
  dish2::RawIntrospectiveState<Spec>
  >;

} // namespace internal

/**
 * Introspective state refers to the collection of simulation-generated sensor
 * values that evolving programs running within each cardinal can access via
 * read-only operations.
 *
 * Each cardinal has an independent copy of each piece of introspective state
 * state. (However, some introspective states representing cell state are set
 * to identical values across cardinals within the same cell.)
 *
 * Each cardinal's introspective state regularly copied and dispatched to that
 * cardinal's neighbor cell, where it serves as read-only extrospective state.
 */
template<typename Spec>
struct IntrospectiveState : public internal::introspective_state_parent_t<Spec>{

  using parent_t = internal::introspective_state_parent_t<Spec>;
  constexpr inline static size_t parent_size = parent_t::GetSize();

  void Assign(const size_t idx, const IntrospectiveState& other) {

    #define DISH2_INTROSPECTIVE_STATE_ASSIGN_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          this->parent_t::template GetByIndex<N>() \
          = static_cast<parent_t>(other).template GetByIndex<N>(); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_INTROSPECTIVE_STATE_ASSIGN_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  void Swap(const size_t idx, IntrospectiveState& other) {

    #define DISH2_INTROSPECTIVE_STATE_SWAP_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          std::swap( \
            this->parent_t::template GetByIndex<N>(), \
            static_cast<parent_t>(other).template GetByIndex<N>() \
          ); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_INTROSPECTIVE_STATE_SWAP_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  static std::string GetLeafTypeName( const size_t idx ) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_INTROSPECTIVE_STATE_NAME_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_t::GetSize() ) { \
          return uitsl::TypeName< \
            typename parent_t::template leaf_t<N> \
          >::Get(); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        }

    emp_assert( idx < parent_t::GetSize() );
    static_assert( parent_t::GetSize() < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_INTROSPECTIVE_STATE_NAME_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

    emp_assert( false, idx );
    __builtin_unreachable();

  }

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_INTROSPECTIVE_STATE_INTROSPECTIVESTATE_HPP_INCLUDE
