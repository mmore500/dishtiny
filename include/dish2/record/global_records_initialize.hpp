#pragma once
#ifndef DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE
#define DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include <sys/stat.h>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace dish2 {

void global_records_initialize() {

  // std::filesystem::create_directories is failing inside Docker container
  // so use mkdir as a backup for now
  const std::string path{ "./drawings" };
  if ( uitsl::is_root() && !std::filesystem::exists( path ) ) {
    emp_always_assert( std::filesystem::create_directories("./drawings") );
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE
