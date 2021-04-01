#pragma once
#ifndef DISH2_CONFIG_PRINT_PWD_HPP_INCLUDE
#define DISH2_CONFIG_PRINT_PWD_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"

#include "../utility/get_hostname.hpp"

namespace dish2 {

void print_pwd() {

  std::cout << "======================================" << '\n';
  std::cout << "|            Where am I?             |" << '\n';
  std::cout << "======================================" << '\n';

  std::cout << dish2::get_hostname() << '\n';
  std::cout << std::filesystem::current_path() << '\n';

  std::cout << "======================================\n" << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_PRINT_PWD_HPP_INCLUDE
