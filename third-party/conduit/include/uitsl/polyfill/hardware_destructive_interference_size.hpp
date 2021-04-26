#pragma once
#ifndef UITSL_POLYFILL_HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE_HPP_INCLUDE
#define UITSL_POLYFILL_HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE_HPP_INCLUDE

#include <cstddef>

#ifdef __EMSCRIPTEN__

namespace std {

  const size_t hardware_destructive_interference_size = 64;

} // namespace std

#endif

#endif // #ifndef UITSL_POLYFILL_HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE_HPP_INCLUDE
