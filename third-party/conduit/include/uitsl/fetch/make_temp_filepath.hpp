#pragma once
#ifndef UITSL_FETCH_MAKE_TEMP_FILEPATH_HPP_INCLUDE
#define UITSL_FETCH_MAKE_TEMP_FILEPATH_HPP_INCLUDE

#include "../polyfill/filesystem.hpp"

#include "make_temp_dirpath.hpp"

namespace uitsl {

inline std::filesystem::path make_temp_filepath() {

  // don't use mkstmp because that leaks file handles
  return uitsl::make_temp_dirpath() / "uitsl_temp_file";

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_MAKE_TEMP_FILEPATH_HPP_INCLUDE
