#pragma once
#ifndef UITSL_PARALLEL_CACHE_LINE_HPP_INCLUDE
#define UITSL_PARALLEL_CACHE_LINE_HPP_INCLUDE

#include <new>
#include <stddef.h>

#include "../polyfill/hardware_destructive_interference_size.hpp"

namespace uitsl {

#ifdef __cpp_lib_hardware_interference_size
  static constexpr size_t CACHE_LINE_SIZE =
    std::hardware_destructive_interference_size;
#else
  static constexpr size_t CACHE_LINE_SIZE = 64;
#endif

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_CACHE_LINE_HPP_INCLUDE
