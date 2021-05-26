#pragma once
#ifndef DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE
#define DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE

#include <cstdlib>
#include <iostream>
#include <string>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "../debug/log_msg.hpp"
#include "../utility/path_exists.hpp"

#include "cfg.hpp"

namespace dish2 {

void setup_config( emp::ArgManager& arg_manager ) {

  for (const auto& arg : emp::vector<std::string>{
    "print_introspective_state_size_minus_one",
    "print_extrospective_state_size_minus_one",
    "print_readable_state_size_minus_one",
    "print_writable_state_size_minus_one",
    "print_reconstituted_lowestroot_numstintroots",
    "print_lowestroot_numstintroots",
    "print_lowestroot"
  } ) while( arg_manager.CallbackArg(arg) );

  // std::filesystem::exists is failing inside Docker container
  // so use stat as a backup for now
  // if ( std::filesystem::exists("dishtiny.cfg") ) {
  if ( dish2::path_exists("dishtiny.cfg") ) {
    dish2::log_msg( "reading configuration from dishtiny.cfg" );
    dish2::cfg.Read( "dishtiny.cfg" );
  } else dish2::log_msg( "no configuration file found" );

  arg_manager.UseCallbacks();

  if ( arg_manager.HasUnused() ) std::exit( EXIT_FAILURE );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE
