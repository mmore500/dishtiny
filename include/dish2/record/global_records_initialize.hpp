#pragma once
#ifndef DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE
#define DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

namespace dish2 {

void global_records_initialize() {

  const std::string path{ "./drawings" };
  const std::string command{ emp::to_string( "mkdir -p ", path ) };
  if ( uitsl::is_root() && !std::filesystem::exists( path ) ) {

    // use mkdir as a fallback
    const bool res = std::filesystem::create_directories( path )
      || std::filesystem::create_directory( path )
      || mkdir( path.c_str(), 0777 ) == 0
      || std::system( command.c_str() ) == 0;

    emp_always_assert(
      res && std::filesystem::exists( path ),
      res, std::filesystem::exists( path )
    );
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE
