#pragma once
#ifndef DISH2_CONFIG_SETUP_HPP_INCLUDE
#define DISH2_CONFIG_SETUP_HPP_INCLUDE

#include <limits>

#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "../run/setup_thread_local_random.hpp"

#include "dump_config.hpp"
#include "print_assets.hpp"
#include "print_config.hpp"
#include "setup_assets.hpp"
#include "setup_config.hpp"

namespace dish2 {

void setup( emp::ArgManager arg_manager ) {

  setup_assets( arg_manager );

  setup_config( arg_manager );

  print_assets();

  print_config();

  dump_config();

  // setup main thread thread local random
  dish2::setup_thread_local_random( std::numeric_limits<uint32_t>::max() );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_SETUP_HPP_INCLUDE
