#pragma once
#ifndef UITSL_MATH_IS_PERFECT_HYPERCUBE_HPP_INCLUDE
#define UITSL_MATH_IS_PERFECT_HYPERCUBE_HPP_INCLUDE

#include <bitset>
#include <cmath>
#include <limits>
#include <ratio>
#include <stddef.h>

#include "../../../third-party/Empirical/include/emp/math/math.hpp"

#include "../debug/safe_cast.hpp"

namespace uitsl {

// adapted from https://stackoverflow.com/questions/1549941/perfect-square-and-perfect-cube
inline bool is_perfect_hypercube(int n, const int dim) {
  if( dim <= 0 ) return false;
  if( n < 0 && dim % 2 == 0 ) return false;

  n = abs(n);

  const int b = std::pow(n, 1.0 / dim);

  return (
    std::pow((double) b, dim) == n
    || std::pow((double) (b+1), dim) == n
  );

}

} // namespace uitsl

#endif // #ifndef UITSL_MATH_IS_PERFECT_HYPERCUBE_HPP_INCLUDE
