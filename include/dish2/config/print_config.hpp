#pragma once
#ifndef DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE
#define DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "cfg.hpp"

namespace dish2 {

void print_config() {

  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  std::cout << "SOURCE DIRECTORY "
    << EMP_STRINGIFY(DISHTINY_SOURCE_DIR_) << std::endl;
  std::cout << "SOURCE HASH " << EMP_STRINGIFY(DISHTINY_HASH_) << std::endl;
  std::cout << "------------------------------" << std::endl;

  dish2::cfg.Write( std::cout );

  std::cout << "==============================\n" << std::endl;

  // todo print directory contents (with hashes?)

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_PRINT_CONFIG_HPP_INCLUDE
