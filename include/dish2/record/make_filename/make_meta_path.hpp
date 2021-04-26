#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_META_PATH_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_META_PATH_HPP_INCLUDE

#include <string>

namespace dish2 {

std::string make_meta_path( const std::string& filename="" ) {
  return std::string("outmeta/") + filename;
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_META_PATH_HPP_INCLUDE
