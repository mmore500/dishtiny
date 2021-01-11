#pragma once
#ifndef DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE
#define DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE

#include <cstdlib>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"

namespace dish2 {

void global_records_initialize() {

  std::filesystem::create_directories("./drawings");

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE
