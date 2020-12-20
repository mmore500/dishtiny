#pragma once
#ifndef DISH2_CONFIG_PRINT_ASSETS_HPP_INCLUDE
#define DISH2_CONFIG_PRINT_ASSETS_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../third-party/conduit/include/uitsl/debug/list_cwd.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "cfg.hpp"

namespace dish2 {

void print_assets() {

  std::cout << "======================================" << std::endl;
  std::cout << "|   What assets am I working with?    |" << std::endl;
  std::cout << "======================================" << std::endl;

  #ifndef __EMSCRIPTEN__
    // hash all files, excluding source directory
    uitsl::err_verify( std::system(
      "find . -path ./dishtiny -prune -o -type f -exec md5sum {} +"
    ) );
  #else
    uitsl::list_cwd();
  #endif // #ifndef __EMSCRIPTEN__

  std::cout << "=======================================\n" << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_PRINT_ASSETS_HPP_INCLUDE
