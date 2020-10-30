#pragma once
#ifndef DISH2_UTIL_NAN_TO_ZERO_HPP_INCLUDE
#define DISH2_UTIL_NAN_TO_ZERO_HPP_INCLUDE

#include <cmath>

namespace dish2 {

template<typename T>
T nan_to_zero(T val) { return std::isnan(val) ? T{} : val; }

} // namespace dish2

#endif // #ifndef DISH2_UTIL_NAN_TO_ZERO_HPP_INCLUDE
