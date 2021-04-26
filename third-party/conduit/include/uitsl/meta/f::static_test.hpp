#pragma once
#ifndef UITSL_META_F__STATIC_TEST_HPP_INCLUDE
#define UITSL_META_F__STATIC_TEST_HPP_INCLUDE

#include <type_traits>

#define uitsl_f_message "T is not a fundamental type"

namespace uitsl {
namespace f { // fundamental type

template<typename T>
class StaticTest : public std::is_fundamental<T> {};

template<typename T>
constexpr bool static_test() { return StaticTest<T>::value; }

} // namespace f
} // namespace uitsl

#endif // #ifndef UITSL_META_F__STATIC_TEST_HPP_INCLUDE
