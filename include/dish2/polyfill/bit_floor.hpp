#pragma once
#ifndef DISH2_POLYFILL_BIT_FLOOR_HPP_INCLUDE
#define DISH2_POLYFILL_BIT_FLOOR_HPP_INCLUDE

#include "bit_width.hpp"

namespace std {

  template< typename T >
  T bit_floor(T x) {
    if (x != 0) return T{1} << (std::bit_width(x) - 1);
    else return 0;
  }

} // namespace std

#endif // #ifndef DISH2_POLYFILL_BIT_FLOOR_HPP_INCLUDE
