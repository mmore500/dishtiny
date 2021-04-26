#pragma once
#ifndef UITSL_META_T__STATIC_TEST_HPP_INCLUDE
#define UITSL_META_T__STATIC_TEST_HPP_INCLUDE

#include <type_traits>

#define uitsl_t_message "T is not a trivially copyable type"

namespace uitsl {
namespace t { // trivial type

template<typename T>
class StaticTest : public std::is_trivially_copyable<T> {};

template<typename T>
constexpr bool static_test() { return StaticTest<T>::value; }

} // namespace t
} // namespace uitsl

#endif // #ifndef UITSL_META_T__STATIC_TEST_HPP_INCLUDE
