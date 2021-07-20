#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_WRITABLESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_WRITABLESTATE_HPP_INCLUDE

#include <algorithm>
#include <cassert>
#include <cstring>
#include <limits>
#include <string>
#include <type_traits>

#include "../../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../../../third-party/signalgp-lite/include/sgpl/utility/ByteEnumeration.hpp"

#include "NopState.hpp"
#include "TransientNopState.hpp"

#include "controller_mapped_state/ControllerMappedState.hpp"

namespace dish2 {

namespace internal {

  template< typename Spec >
  using writable_state_parent_t = uitsl::PodInternalNode<
    dish2::ControllerMappedState< Spec >,
    dish2::NopState< Spec >,
    dish2::TransientNopState< Spec >
  >;

} // namespace internal

/**
 * Writable state refers to the collection of output values that evolving
 * programs running within each cardinal can write to and read from.
 *
 * Some of these outputs enable interaction with the simulation (i.e., control
 * phenotypic characteristics). Each cardinal has an independent copy of each
 * piece of writable state state.
 */
template< typename Spec >
struct WritableState : public dish2::internal::writable_state_parent_t<Spec> {

  using parent_t = dish2::internal::writable_state_parent_t<Spec>;
  constexpr inline static size_t parent_size = parent_t::GetSize();

  void AddTo(const size_t idx, const float val) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_ADD_TO_STATE_CASE_PAYLOAD(N) \
    case N: \
      if constexpr ( N  < parent_size ) { \
        using target_t = std::decay_t< decltype( \
          this->parent_t::template GetByIndex<N>() \
        ) >; \
        const target_t clamped = uitsl::clamp_cast< target_t >( val ); \
        this->parent_t::template GetByIndex<N>() += clamped; \
      } else { \
        assert( false && N ); \
        __builtin_unreachable(); \
      } \
    break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH( DISH2_ADD_TO_STATE_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  void Multiply(const size_t idx, const float val) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_MULTIPLY_STATE_CASE_PAYLOAD(N) \
    case N: \
      if constexpr ( N  < parent_size ) { \
        using target_t = std::decay_t< decltype( \
          this->parent_t::template GetByIndex<N>() \
        ) >; \
        const target_t clamped = uitsl::clamp_cast< target_t >( val ); \
        this->parent_t::template GetByIndex<N>() *= clamped; \
      } else { \
        assert( false && N ); \
        __builtin_unreachable(); \
      } \
    break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH( DISH2_MULTIPLY_STATE_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  void Write(const size_t idx, const float val) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_WRITABLE_STATE_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          using target_t = std::decay_t< decltype( \
            this->parent_t::template GetByIndex<N>() \
          ) >; \
          const target_t clamped = uitsl::clamp_cast< target_t >( val ); \
          this->parent_t::template GetByIndex<N>() = clamped; \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH( DISH2_WRITABLE_STATE_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  void Assign(const size_t idx, const WritableState& other) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_WRITABLE_STATE_ASSIGN_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          this->parent_t::template GetByIndex<N>() \
          = static_cast<parent_t>(other).template GetByIndex<N>(); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_WRITABLE_STATE_ASSIGN_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  void Swap(const size_t idx, WritableState& other) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_WRITABLE_STATE_SWAP_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          std::swap( \
            this->parent_t::template GetByIndex<N>(), \
            static_cast<parent_t>(other).template GetByIndex<N>() \
          ); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_WRITABLE_STATE_SWAP_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  static std::string GetLeafTypeName( const size_t idx ) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_WRITABLE_STATE_NAME_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          return uitsl::TypeName< \
            typename parent_t::template leaf_t<N> \
          >::Get(); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        }

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_WRITABLE_STATE_NAME_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

    emp_assert( false, idx );
    __builtin_unreachable();

  }

  static size_t GetLeafIndex( const size_t idx ) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_WRITABLE_STATE_INDEX_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          return parent_t::template GetLeafIndex<N>(); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        }

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_WRITABLE_STATE_INDEX_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
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

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_WRITABLESTATE_HPP_INCLUDE
