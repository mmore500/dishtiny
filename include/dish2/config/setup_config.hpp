#pragma once
#ifndef DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE
#define DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE

#include <cstdlib>

#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "cfg.hpp"

namespace dish2 {

void setup_config( emp::ArgManager& arg_manager ) {

  dish2::cfg.Read( "dishtiny.cfg" );

  arg_manager.UseCallbacks();

  if ( arg_manager.HasUnused() ) std::exit( EXIT_FAILURE );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_SETUP_CONFIG_HPP_INCLUDE
