#pragma once
#ifndef SGPL_LIBRARY_OPLOOKUP_HPP_INCLUDE
#define SGPL_LIBRARY_OPLOOKUP_HPP_INCLUDE

#include <cassert>
#include <unordered_map>

#include "../../../third-party/conduit/include/uitsl/meta/tuple_has_type.hpp"
#include "../../../third-party/conduit/include/uitsl/meta/tuple_index.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../operations/actions/Nop.hpp"
#include "../utility/ByteEnumeration.hpp"
#include "../utility/count_operation_random_touches.hpp"

namespace sgpl {

template<typename Library>
class OpLookup {

  std::unordered_map<std::string, unsigned char> table;

  using library_parent_t = typename Library::parent_t;

public:

  OpLookup() {

    #define SGPL_OP_LOOKUP_PAYLOAD(N) \
      if constexpr (N < Library::GetSize()) { \
        using Operation = typename Library::template Operation<N>; \
        table[ Operation::name() ] = N; \
      } \

    static_assert( Library::GetSize() < 256 );

    EMP_WRAP_EACH( SGPL_OP_LOOKUP_PAYLOAD, SGPL_BYTE_ENUMERATION )

    emp_assert(
      table.size() == Library::GetSize(), table.size(), Library::GetSize()
    );

  }

  unsigned char GetOpCode(const std::string op_name) const {
    return table.at(op_name);
  }

  static std::string GetOpName(const size_t op_code) {

    // can't use emp_assert due to obsucre macro error
    #define SGPL_OP_NAME_PAYLOAD(N) \
      case N: \
        if constexpr (N < Library::GetSize()) { \
          using Operation = typename Library::template Operation<N>; \
          return Operation::name(); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    static_assert( Library::GetSize() < 256 );

    switch( op_code ) {

      EMP_WRAP_EACH( SGPL_OP_NAME_PAYLOAD, SGPL_BYTE_ENUMERATION )

      default:
        emp_assert( false, op_code );
        __builtin_unreachable();

    }

    emp_assert(false, op_code);
    __builtin_unreachable();

  }

  template< typename Spec >
  static size_t GetOpNumRngTouches(const size_t op_code) {

    // can't use emp_assert due to obsucre macro error
    #define SGPL_OP_NUM_RNG_TOUCHES_PAYLOAD(N) \
      case N: \
        if constexpr (N < Library::GetSize()) { \
          using Operation = typename Library::template Operation<N>; \
          return sgpl::count_operation_random_touches< Operation, Spec >(); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    static_assert( Library::GetSize() < 256 );

    switch( op_code ) {

      EMP_WRAP_EACH( SGPL_OP_NUM_RNG_TOUCHES_PAYLOAD, SGPL_BYTE_ENUMERATION )

      default:
        emp_assert(false, op_code);
        __builtin_unreachable();

    }

    emp_assert(false, op_code);
    __builtin_unreachable();

  }

  static size_t GetNopOpCode( const size_t num_rng_touches ) {

    // can't use emp_assert due to obsucre macro error
    #define SGPL_NOP_OP_CODE_PAYLOAD(N) \
      case N: \
        if constexpr ( \
          uitsl::tuple_has_type< sgpl::Nop<N>, library_parent_t >::value \
        )  return uitsl::tuple_index< sgpl::Nop<N>, library_parent_t >::value; \
        else if constexpr ( \
          uitsl::tuple_has_type< sgpl::Nop<N, 0>, library_parent_t >::value \
        )  return uitsl::tuple_index<sgpl::Nop<N, 0>, library_parent_t>::value; \
        else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( num_rng_touches < 256 );

    switch( num_rng_touches ) {

      EMP_WRAP_EACH( SGPL_NOP_OP_CODE_PAYLOAD, SGPL_BYTE_ENUMERATION )

      default:
        emp_assert(false, num_rng_touches);
        __builtin_unreachable();

    }

    emp_assert(false, num_rng_touches);
    __builtin_unreachable();

  }

  static size_t GetOpPrevalence(const size_t op_code) {

    // can't use emp_assert due to obsucre macro error
    #define SGPL_OP_PREVALENCE_PAYLOAD(N) \
      case N: \
        if constexpr (N < Library::GetSize()) { \
          using Operation = typename Library::template Operation<N>; \
          return Operation::prevalence(); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    static_assert( Library::GetSize() < 256 );

    switch( op_code ) {

      EMP_WRAP_EACH( SGPL_OP_PREVALENCE_PAYLOAD, SGPL_BYTE_ENUMERATION )

      default:
        emp_assert(false, op_code);
        __builtin_unreachable();

    }

    emp_assert(false, op_code);
    __builtin_unreachable();

  }

  template< typename Instruction >
  static auto GetOpDescriptors(
    const size_t op_code, const Instruction& instruction
  ) {

    // can't use emp_assert due to obsucre macro error
    #define SGPL_OP_GET_DESCRIPTORS(N) \
      case N: \
        if constexpr (N < Library::GetSize()) { \
          using Operation = typename Library::template Operation<N>; \
          return Operation::descriptors( instruction ); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    static_assert( Library::GetSize() < 256 );

    switch( op_code ) {

      EMP_WRAP_EACH(
        SGPL_OP_GET_DESCRIPTORS, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert(false, op_code);
        __builtin_unreachable();

    }

    emp_assert(false, op_code);
    __builtin_unreachable();

  }

  template< typename Instruction >
  static auto GetOpCategories(
    const size_t op_code, const Instruction& instruction
  ) {

    // can't use emp_assert due to obsucre macro error
    #define SGPL_OP_GET_CATEGORIES(N) \
      case N: \
        if constexpr (N < Library::GetSize()) { \
          using Operation = typename Library::template Operation<N>; \
          return Operation::categories( instruction ); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    static_assert( Library::GetSize() < 256 );

    switch( op_code ) {

      EMP_WRAP_EACH(
        SGPL_OP_GET_CATEGORIES, SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert(false, op_code);
        __builtin_unreachable();

    }

    emp_assert(false, op_code);
    __builtin_unreachable();

  }

};

} // namespace sgpl

#endif // #ifndef SGPL_LIBRARY_OPLOOKUP_HPP_INCLUDE
