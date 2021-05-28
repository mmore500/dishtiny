#pragma once
#ifndef DISH2_CONFIG_PRINT_ASSETS_HPP_INCLUDE
#define DISH2_CONFIG_PRINT_ASSETS_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/list_cwd.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "../debug/log_tee.hpp"

#include "cfg.hpp"

namespace dish2 {

void print_assets() {

  dish2::log_tee << "======================================" << '\n';
  dish2::log_tee << "|   What assets am I working with?    |" << '\n';
  dish2::log_tee << "======================================" << '\n';

  #ifndef __EMSCRIPTEN__
    // hash all files, excluding source directory
    uitsl::err_verify( std::system(
      "find . -path ./dishtiny -prune -o -type f -exec md5sum {} +"
    ) );
  #else
    uitsl::list_cwd(dish2::log_tee);
  #endif // #ifndef __EMSCRIPTEN__

  dish2::log_tee << "=======================================\n" << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_PRINT_ASSETS_HPP_INCLUDE
