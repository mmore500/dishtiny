#pragma once
#ifndef DISH2_UTILITY_STRLEN_HPP_INCLUDE
#define DISH2_UTILITY_STRLEN_HPP_INCLUDE

#include <cstring>
#include <string>

#include "../../../third-party/conduit/include/uitsl/polyfill/is_constant_evaluated.hpp"

namespace dish2 {

// constexpr implementation of strlen
// adapted from https://isocpp.org/blog/2014/09/strlen-constexpr
constexpr int strlen(const char* str) {
  if ( std::is_constant_evaluated() ) return *str ? 1 + strlen(str + 1) : 0;
  else return std::strlen( str );
}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_STRLEN_HPP_INCLUDE
