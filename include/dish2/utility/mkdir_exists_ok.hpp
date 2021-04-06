#pragma once
#ifndef DISH2_UTILITY_MKDIR_EXISTS_OK_HPP_INCLUDE
#define DISH2_UTILITY_MKDIR_EXISTS_OK_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include <sys/stat.h>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

namespace dish2 {

// run mkdir with lots of backup options
void mkdir_exists_ok(const std::string& path) {

  if ( std::filesystem::exists( path ) ) return;

  const std::string command{ emp::to_string( "mkdir -p ", path ) };

  // use ||'s to provide fallback options
  const bool res = std::filesystem::create_directories( path )
    || std::filesystem::create_directory( path )
    || mkdir( path.c_str(), 0777 ) == 0
    || std::system( command.c_str() ) == 0;

  emp_always_assert(
    res && std::filesystem::exists( path ),
    res, std::filesystem::exists( path )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_MKDIR_EXISTS_OK_HPP_INCLUDE
