#pragma once
#ifndef DISH2_POLYFILL_BIT_WIDTH_HPP_INCLUDE
#define DISH2_POLYFILL_BIT_WIDTH_HPP_INCLUDE

namespace std {

  template< typename T >
  size_t bit_width(T x) {
    size_t res{};
    while (x) {
      x >>= 1;
      ++res;
    }
    return res;
  }

} // namespace std

#endif // #ifndef DISH2_POLYFILL_BIT_WIDTH_HPP_INCLUDE
