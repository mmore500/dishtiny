#pragma once
#ifndef DISH2_CONFIG_SETUP_HPP_INCLUDE
#define DISH2_CONFIG_SETUP_HPP_INCLUDE

#include <limits>
#include <string>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/mpi_flex_guard.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "../run/setup_thread_local_random.hpp"

#include "dump_config_csv.hpp"
#include "dump_config.hpp"
#include "print_assets.hpp"
#include "print_config.hpp"
#include "print_extrospective_states.hpp"
#include "print_introspective_states.hpp"
#include "print_pwd.hpp"
#include "print_readable_states.hpp"
#include "print_writable_states.hpp"
#include "setup_assets.hpp"
#include "setup_config.hpp"

namespace dish2 {

template< typename Spec >
void setup( emp::ArgManager arg_manager ) {

  if (
    (arg_manager.ViewArg("N_THREADS").empty() &&  dish2::cfg.N_THREADS() == 1)
    || arg_manager.ViewArg("N_THREADS").back() == emp::vector<std::string>{"1"}
  ) uitsl::mpi_flex_guard.InitSingleThread();
  else uitsl::mpi_flex_guard.InitMultithread();

  setup_assets( arg_manager );

  setup_config( arg_manager );

  if ( uitsl::is_root() ) {
    print_pwd();
    print_assets();
    print_config();
    print_extrospective_states<Spec>();
    print_introspective_states<Spec>();
    print_readable_states<Spec>();
    print_writable_states<Spec>();
  }

  dump_config();
  dump_config_csv();

  // setup main thread thread local random
  dish2::setup_thread_local_random();

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_SETUP_HPP_INCLUDE
