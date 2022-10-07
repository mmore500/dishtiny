#pragma once
#ifndef DISH2_CONFIG_VECTOR_STREAM_OPS_HPP_INCLUDE
#define DISH2_CONFIG_VECTOR_STREAM_OPS_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

namespace std {

  // A crude, generic printing function for arrays.
  template <typename T>
  inline std::ostream & operator<<(
    std::ostream & out,
    const emp::vector<T> & v
  ) {
    for (const T & x : v) out << x << " ";
    return out;
  }

  template <typename T>
  std::istream & operator>>(std::istream & is, emp::vector<T> & v) {
    T x;
    while(is >> x) v.push_back(x);
    return is;
  }

} // namespace std

#endif // #ifndef DISH2_CONFIG_VECTOR_STREAM_OPS_HPP_INCLUDE
