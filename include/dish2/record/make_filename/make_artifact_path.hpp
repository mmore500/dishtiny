#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_ARTIFACT_PATH_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_ARTIFACT_PATH_HPP_INCLUDE

#include <string>

namespace dish2 {

std::string make_artifact_path( const std::string& filename="" ) {
  return std::string("outartifacts/") + filename;
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_ARTIFACT_PATH_HPP_INCLUDE
