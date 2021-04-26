#pragma once
#ifndef UITSL_ALGORITHM_NONFINITE_TO_ZERO_HPP_INCLUDE
#define UITSL_ALGORITHM_NONFINITE_TO_ZERO_HPP_INCLUDE

#include <cmath>

namespace uitsl {

template<typename T>
T nonfinite_to_zero(const T val) { return std::isfinite(val) ? val : T{}; }

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_NONFINITE_TO_ZERO_HPP_INCLUDE
