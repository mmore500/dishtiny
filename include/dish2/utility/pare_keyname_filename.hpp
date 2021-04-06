#pragma once
#ifndef DISH2_UTILITY_PARE_KEYNAME_FILENAME_HPP_INCLUDE
#define DISH2_UTILITY_PARE_KEYNAME_FILENAME_HPP_INCLUDE

#include <string>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"

#include "pare_keyname_filename_impl.hpp"

#include "../debug/log_msg.hpp"

namespace dish2 {

std::string pare_keyname_filename(
  const std::string& original_filename, const std::filesystem::path& path="./"
) {

  const std::string res = dish2::pare_keyname_filename_impl(
    original_filename, path
  );

  if ( res != original_filename ) {
    dish2::log_msg( "paring filename ", original_filename, " -> ", res );
  }

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_PARE_KEYNAME_FILENAME_HPP_INCLUDE
