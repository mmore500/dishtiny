#pragma once
#ifndef DISH2_POLYFILL_COUNTR_ZERO_HPP_INCLUDE
#define DISH2_POLYFILL_COUNTR_ZERO_HPP_INCLUDE

namespace std {

  // adapted from https://johnmcfarlane.github.io/cnl/bit_8h_source.html
  template< typename T >
  size_t countr_zero(T x) {
    return (x & 1)
      ? 0
      : countr_zero<T>(static_cast<T>(x >> 1)) + 1
    ;
  }

} // namespace std

#endif // #ifndef DISH2_POLYFILL_COUNTR_ZERO_HPP_INCLUDE
