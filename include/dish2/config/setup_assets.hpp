#pragma once
#ifndef DISH2_CONFIG_SETUP_ASSETS_HPP_INCLUDE
#define DISH2_CONFIG_SETUP_ASSETS_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

namespace dish2 {

void setup_assets( emp::ArgManager& arg_manager ) {

  if ( uitsl::is_root() ) while( arg_manager.CallbackArg( "autoinstall" ) );
  else while( arg_manager.UseArg( "autoinstall" ) );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_SETUP_ASSETS_HPP_INCLUDE
