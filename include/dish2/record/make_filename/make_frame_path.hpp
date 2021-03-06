#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_FRAME_PATH_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_FRAME_PATH_HPP_INCLUDE

#include <string>

namespace dish2 {

std::string make_frame_path( const std::string& filename="" ) {
  return std::string("outframes/") + filename;
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_FRAME_PATH_HPP_INCLUDE
