#pragma once
#ifndef UITSL_POLYFILL_ISFINITE_HPP_INCLUDE
#define UITSL_POLYFILL_ISFINITE_HPP_INCLUDE

#include <cmath>

#ifdef __EMSCRIPTEN__

namespace std {

bool isfinite( const float arg ) {
  return std::isfinite( static_cast<double>( arg ) );
}

} // namespace std

#endif // #ifdef __EMSCRIPTEN__

#endif // #ifndef UITSL_POLYFILL_ISFINITE_HPP_INCLUDE
