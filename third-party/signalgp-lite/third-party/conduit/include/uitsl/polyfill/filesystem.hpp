#pragma once
#ifndef UITSL_POLYFILL_FILESYSTEM_HPP_INCLUDE
#define UITSL_POLYFILL_FILESYSTEM_HPP_INCLUDE

#if defined(__EMSCRIPTEN__) \
  && __EMSCRIPTEN_major__ == 1 && __EMSCRIPTEN_minor__ <= 38

#include <experimental/filesystem>

namespace std {
namespace filesystem {

  using namespace std::experimental::filesystem;

} // namespace filesystem
} // namespace std

#else

#include <filesystem>

#endif

#endif // #ifndef UITSL_POLYFILL_FILESYSTEM_HPP_INCLUDE
