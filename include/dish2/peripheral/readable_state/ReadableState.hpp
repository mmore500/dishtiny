#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_READABLESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_READABLESTATE_HPP_INCLUDE

#include <cstring>
#include <limits>

#include "../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../third-party/Empirical/include/emp/base/macros.hpp"
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
  constexpr inline static size_t parent_size = parent_t::GetSize();

  // https://stackoverflow.com/a/63046442
  template<size_t Templateify=0>
  double Read(const size_t idx) const {

    #define DISH2_READABLE_STATE_CASE_PAYLOAD(N) \
      case N: \
        if constexpr (N < parent_t::GetSize()){ \
          return this->template Get<N + Templateify>(); \
        } else { \
          emp_assert( false, N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( idx < parent_t::GetSize() );
    static_assert( parent_t::GetSize() < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH( DISH2_READABLE_STATE_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

    emp_assert( false, idx );
    __builtin_unreachable();

  }

  void Assign(const size_t idx, const ReadableState& other) {

    #define DISH2_READABLE_STATE_ASSIGN_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          this->parent_t::template GetByIndex<N>() \
          = static_cast<parent_t>(other).template GetByIndex<N>(); \
        } else { \
          emp_assert( false, N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_READABLE_STATE_ASSIGN_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  void Swap(const size_t idx, ReadableState& other) {

    #define DISH2_READABLE_STATE_SWAP_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          std::swap( \
            this->parent_t::template GetByIndex<N>(), \
            static_cast<parent_t>(other).template GetByIndex<N>() \
          ); \
        } else { \
          emp_assert( false, N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_READABLE_STATE_SWAP_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  static std::string GetLeafTypeName( const size_t idx ) {

    #define DISH2_READABLE_STATE_NAME_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_t::GetSize() ) { \
          return uitsl::TypeName< \
            typename parent_t::template leaf_t<N> \
          >::Get(); \
        } else { \
          emp_assert( false, N ); \
          __builtin_unreachable(); \
        }

    emp_assert( idx < parent_t::GetSize() );
    static_assert( parent_t::GetSize() < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_READABLE_STATE_NAME_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

    emp_assert( false, idx );
    __builtin_unreachable();

  }

  static size_t GetLeafIndex( const size_t idx ) {

    #define DISH2_READABLE_STATE_INDEX_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_t::GetSize() ) { \
          return parent_t::template GetLeafIndex<N>(); \
        } else { \
          emp_assert( false, N ); \
          __builtin_unreachable(); \
        }

    emp_assert( idx < parent_t::GetSize() );
    static_assert( parent_t::GetSize() < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_READABLE_STATE_INDEX_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
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

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_READABLESTATE_HPP_INCLUDE
