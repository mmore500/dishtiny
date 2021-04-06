#pragma once
#ifndef DISH2_CONFIG_PRINT_PWD_HPP_INCLUDE
#define DISH2_CONFIG_PRINT_PWD_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"

#include "../debug/log_tee.hpp"
#include "../utility/get_hostname.hpp"

namespace dish2 {

void print_pwd() {

  dish2::log_tee << "======================================" << '\n';
  dish2::log_tee << "|            Where am I?             |" << '\n';
  dish2::log_tee << "======================================" << '\n';

  dish2::log_tee << dish2::get_hostname() << '\n';
  dish2::log_tee << std::filesystem::current_path() << '\n';

  dish2::log_tee << "======================================\n" << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_PRINT_PWD_HPP_INCLUDE
