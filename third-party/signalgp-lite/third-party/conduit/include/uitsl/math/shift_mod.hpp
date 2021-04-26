#pragma once
#ifndef UITSL_MATH_SHIFT_MOD_HPP_INCLUDE
#define UITSL_MATH_SHIFT_MOD_HPP_INCLUDE

#include <cstdlib>

#include "math_utils.hpp"

namespace uitsl {

/// Calculate n % d.
size_t shift_mod(size_t n, size_t d) {
  emp_assert( uitsl::is_power_of_two( d ) );
  return ( n & (d - 1) );
}

} // namespace uitsl

#endif // #ifndef UITSL_MATH_SHIFT_MOD_HPP_INCLUDE
