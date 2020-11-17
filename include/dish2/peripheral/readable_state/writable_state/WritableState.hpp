#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_WRITABLESTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_WRITABLESTATE_HPP_INCLUDE

#include <string>
#include <type_traits>

#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../../third-party/conduit/include/uitsl/datastructs/PodLeafNode.hpp"
#include "../../../../../third-party/Empirical/source/base/macros.h"
#include "../../../../../third-party/signalgp-lite/include/sgpl/utility/ByteEnumeration.hpp"

#include "NopState.hpp"

#include "controller_mapped_state/ControllerMappedState.hpp"

namespace dish2 {

namespace internal {

  template< typename Spec >
  using writable_state_parent_t = uitsl::PodInternalNode<
    dish2::ControllerMappedState< Spec >,
    dish2::NopState< Spec >
  >;

} // namespace internal

template< typename Spec >
struct WritableState : public dish2::internal::writable_state_parent_t<Spec> {

  using parent_t = dish2::internal::writable_state_parent_t<Spec>;
  constexpr inline static size_t parent_size = parent_t::GetSize();

  void AddTo(const size_t idx, const float val) {

    #define DISH2_ADD_TO_STATE_CASE_PAYLOAD(N) \
    case N: \
      if constexpr ( N  < parent_size ) { \
        using target_t = std::decay_t< decltype( \
          this->parent_t::template GetByIndex<N>() \
        ) >; \
        const target_t clamped = uitsl::clamp_cast< std::decay_t< decltype( \
          this->parent_t::template GetByIndex<N>() \
        ) > >( val ); \
        this->parent_t::template GetByIndex<N>() += clamped; \
      } \
    break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH( DISH2_ADD_TO_STATE_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

    }

  }

  void Multiply(const size_t idx, const float val) {

    #define DISH2_MULTIPLY_STATE_CASE_PAYLOAD(N) \
    case N: \
      if constexpr ( N  < parent_size ) { \
        using target_t = std::decay_t< decltype( \
          this->parent_t::template GetByIndex<N>() \
        ) >; \
        const target_t clamped = uitsl::clamp_cast< std::decay_t< decltype( \
          this->parent_t::template GetByIndex<N>() \
        ) > >( val ); \
        this->parent_t::template GetByIndex<N>() *= clamped; \
      } \
    break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH( DISH2_MULTIPLY_STATE_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

    }

  }

  void Write(const size_t idx, const float val) {

    #define DISH2_WRITABLE_STATE_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          using target_t = std::decay_t< decltype( \
            this->parent_t::template GetByIndex<N>() \
          ) >; \
          const target_t clamped = uitsl::clamp_cast< std::decay_t< decltype( \
            this->parent_t::template GetByIndex<N>() \
          ) > >( val ); \
          this->parent_t::template GetByIndex<N>() = clamped; \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH( DISH2_WRITABLE_STATE_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

    }

  }

  static std::string GetLeafTypeName( const size_t idx ) {

    #define DISH2_WRITABLE_STATE_NAME_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          return uitsl::TypeName< \
            typename parent_t::template leaf_t<N> \
          >::Get(); \
        }

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_WRITABLE_STATE_NAME_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION
      )

    }

    return "bad idx";

  }

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_WRITABLESTATE_HPP_INCLUDE
