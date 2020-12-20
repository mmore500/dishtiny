#pragma once
#ifndef DISH2_CONFIG_SETUP_ASSETS_HPP_INCLUDE
#define DISH2_CONFIG_SETUP_ASSETS_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

namespace dish2 {

void setup_assets( emp::ArgManager& arg_manager ) {

  while( arg_manager.CallbackArg( "autoinstall" ) );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_SETUP_ASSETS_HPP_INCLUDE
