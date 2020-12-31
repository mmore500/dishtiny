#pragma once
#ifndef DISH2_RECORD_INITIALIZE_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_INITIALIZE_DRAWINGS_HPP_INCLUDE

#include <cstdlib>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"

namespace dish2 {

void initialize_drawings() {

  std::filesystem::create_directories("./drawings");

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_INITIALIZE_DRAWINGS_HPP_INCLUDE
