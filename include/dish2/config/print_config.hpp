#pragma once
#ifndef DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE
#define DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "cfg.hpp"

namespace dish2 {

void print_config() {

  std::cout << "==============================" << '\n';
  std::cout << "|    How am I configured?    |" << '\n';
  std::cout << "==============================" << '\n';
  std::cout << "SOURCE DIRECTORY "
    << EMP_STRINGIFY(DISHTINY_SOURCE_DIR_) << '\n';
  std::cout << "SOURCE HASH " << EMP_STRINGIFY(DISHTINY_HASH_) << '\n';
  std::cout << "------------------------------" << '\n';

  dish2::cfg.Write( std::cout );

  std::cout << "==============================\n" << '\n';

  // todo print directory contents (with hashes?)

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE
