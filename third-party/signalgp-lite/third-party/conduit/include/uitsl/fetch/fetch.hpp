#pragma once
#ifndef UITSL_FETCH_FETCH_HPP_INCLUDE
#define UITSL_FETCH_FETCH_HPP_INCLUDE

#include <cstdio>
#include <cstdlib>
#include <string>

#include "../polyfill/filesystem.hpp"

#ifdef __EMSCRIPTEN__
  #include "fetch_web.hpp"
#else
  #include "fetch_native.hpp"
#endif

namespace uitsl {

/*
 * Synchronously fetch data from a url to a temporary file,
 * return temporary file path. Works natively and with Emscripten.
 * Requires -lcurl when compiling for native. Requires `xmlhttprequest` when
 * running with Node.js.
 */
inline std::filesystem::path fetch( const std::string& url ) {
  #ifdef __EMSCRIPTEN__
    return uitsl::fetch_web( url );
  #else
    return uitsl::fetch_native( url );
  #endif
}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_FETCH_HPP_INCLUDE
