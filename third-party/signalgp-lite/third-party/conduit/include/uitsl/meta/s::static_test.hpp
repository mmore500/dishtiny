#pragma once
#ifndef UITSL_META_S__STATIC_TEST_HPP_INCLUDE
#define UITSL_META_S__STATIC_TEST_HPP_INCLUDE

#include <type_traits>

#include "HasMemberFunction.hpp"
#include "HasMemberType.hpp"

#define uitsl_s_message "T is not a fundamental type"

namespace uitsl {

UITSL_GENERATE_HAS_MEMBER_FUNCTION( data );
UITSL_GENERATE_HAS_MEMBER_FUNCTION( size );
UITSL_GENERATE_HAS_MEMBER_TYPE( value_type );

namespace s { // span type

template<typename T>
class StaticTest {

  static constexpr bool test() {
    if constexpr ( std::is_class<T>::value ) {
      return uitsl::HasMemberType_value_type<T>::value
      && uitsl::HasMemberFunction_data<T, typename T::value_type*()>::value
      && uitsl::HasMemberFunction_size<T, size_t()>::value;
    } else return false;
  }

public:

  static constexpr bool value = test();

};

template<typename T>
constexpr bool static_test() { return StaticTest<T>::value; }

} // namespace s
} // namespace uitsl

#endif // #ifndef UITSL_META_S__STATIC_TEST_HPP_INCLUDE
