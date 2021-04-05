#pragma once
#ifndef DISH2_RUN_THREAD_TRY_END_SNAPSHOT_HPP_INCLUDE
#define DISH2_RUN_THREAD_TRY_END_SNAPSHOT_HPP_INCLUDE

#include <chrono>
#include <iostream>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../config/cfg.hpp"
#include "../config/thread_idx.hpp"
#include "../utility/try_with_timeout.hpp"
#include "../world/ThreadWorld.hpp"

#include "thread_end_snapshot.hpp"

namespace dish2 {

template<typename Spec>
void thread_try_end_snapshot( dish2::ThreadWorld<Spec>& thread_world ) {

  if ( !cfg.DATA_DUMP() ) {
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " skipping end snapshot" << '\n';
    return;
  }

  const bool res = dish2::try_with_timeout(
    [&](){ dish2::thread_end_snapshot<Spec>(thread_world); },
    std::chrono::duration<double>(
      dish2::cfg.END_SNAPSHOT_TIMEOUT_SECONDS()
    )
  );

  if ( res ) {
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " end snapshot success" << '\n';
  } else {
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " end snapshot timed out" << '\n';
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_TRY_END_SNAPSHOT_HPP_INCLUDE
