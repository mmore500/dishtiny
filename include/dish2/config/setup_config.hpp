#pragma once
#ifndef DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE
#define DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "../utility/file_exists.hpp"

#include "cfg.hpp"

namespace dish2 {

void setup_config( emp::ArgManager& arg_manager ) {

  // std::filesystem::exists is failing inside Docker container
  // so use stat as a backup for now
  // if ( std::filesystem::exists("dishtiny.cfg") ) {
  if ( dish2::file_exists("dishtiny.cfg") ) {
    std::cout << "reading configuration from dishtiny.cfg" << std::endl;
    dish2::cfg.Read( "dishtiny.cfg" );
  } else std::cout << "no configuration file found" << std::endl;

  arg_manager.UseCallbacks();

  if ( arg_manager.HasUnused() ) std::exit( EXIT_FAILURE );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE
