#pragma once
#ifndef UITSL_ALGORITHM_NAN_TO_ZERO_HPP_INCLUDE
#define UITSL_ALGORITHM_NAN_TO_ZERO_HPP_INCLUDE

#include <cmath>

namespace uitsl {

template<typename T>
T nan_to_zero(const T val) { return std::isnan(val) ? T{} : val; }

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_NAN_TO_ZERO_HPP_INCLUDE
