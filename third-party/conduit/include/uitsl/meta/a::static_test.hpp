#pragma once
#ifndef UITSL_META_A__STATIC_TEST_HPP_INCLUDE
#define UITSL_META_A__STATIC_TEST_HPP_INCLUDE

#include <type_traits>

#define uitsl_a_message "T is neither copy nor move constructible"

namespace uitsl {
namespace a { // any type

template<typename T>
struct StaticTest {

  static constexpr bool value = (
    std::is_copy_constructible<T>::value
    || std::is_move_constructible<T>::value
  );

};

template<typename T>
constexpr bool static_test() { return StaticTest<T>::value; }

} // namespace a
} // namespace uitsl

#endif // #ifndef UITSL_META_A__STATIC_TEST_HPP_INCLUDE
