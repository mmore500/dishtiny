#pragma once
#ifndef DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE
#define DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "../debug/log_tee.hpp"

#include "cfg.hpp"

namespace dish2 {

void print_config() {

  dish2::log_tee << "==============================" << '\n';
  dish2::log_tee << "|    How am I configured?    |" << '\n';
  dish2::log_tee << "==============================" << '\n';
  dish2::log_tee << "SOURCE DIRECTORY "
    << EMP_STRINGIFY(DISHTINY_SOURCE_DIR_) << '\n';
  dish2::log_tee << "SOURCE HASH " << EMP_STRINGIFY(DISHTINY_HASH_) << '\n';
  dish2::log_tee << "------------------------------" << '\n';

  dish2::cfg.Write( dish2::log_tee );

  dish2::log_tee << "==============================\n" << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE
