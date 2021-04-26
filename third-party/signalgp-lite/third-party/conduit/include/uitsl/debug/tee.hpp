#pragma once
#ifndef UITSL_DEBUG_TEE_HPP_INCLUDE
#define UITSL_DEBUG_TEE_HPP_INCLUDE

#include <iostream>
#include <utility>

namespace uitsl {

/*
 * Conveniently log a value, including R-values.
 * Adapted from
 * https://www.fluentcpp.com/2017/09/29/tee-getting-precious-debug-info-easily/.
 */
template <typename T>
decltype(auto) tee(T&& value) {

  std::cout << value << '\n';
  return std::forward<T>(value);

}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_TEE_HPP_INCLUDE
