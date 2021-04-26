#pragma once
#ifndef UITSL_DEBUG_COMPARE_FILES_HPP_INCLUDE
#define UITSL_DEBUG_COMPARE_FILES_HPP_INCLUDE

#include <fstream>
#include <string>

#include "compare_streams.hpp"

namespace uitsl {

/*
 * Return true if files are equal.
 */
inline bool compare_files(const std::string& p1, const std::string& p2) {
  // adapted from https://stackoverflow.com/a/37575457
  std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

  return uitsl::compare_streams(f1, f2);
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_COMPARE_FILES_HPP_INCLUDE
