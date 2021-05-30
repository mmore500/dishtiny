#pragma once
#ifndef DISH2_DEBUG_MAKE_LOG_ENTRY_BOILERPLATE_HPP_INCLUDE
#define DISH2_DEBUG_MAKE_LOG_ENTRY_BOILERPLATE_HPP_INCLUDE

#include <string>
#include <thread>

#include <unistd.h>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/thread_idx.hpp"
#include "../utility/mkdir_exists_ok.hpp"
#include "../utility/static_coarse_timer.hpp"

namespace dish2 {

std::string make_log_entry_boilerplate() {

  auto dup = sgpl::tlrand.Get();

  return emp::to_string(
    "[proc ", uitsl::get_proc_id(), "] ",
    "[thread ", dish2::thread_idx, "] ",
    "[sysproc ", getpid(), "] ",
    "[systhread ", std::this_thread::get_id(), "] ",
    "[rng_state ", dup.GetUInt(), "] ",
    '\n',
    "└──( ", dish2::static_coarse_timer.GetElapsed().count(), "s ) >> "
  );
}

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_MAKE_LOG_ENTRY_BOILERPLATE_HPP_INCLUDE
