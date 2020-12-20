#pragma once
#ifndef DISH2_RUN_PRINT_PROGRESS_HPP_INCLUDE
#define DISH2_RUN_PRINT_PROGRESS_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void print_progress(
  const size_t thread_idx,
  const dish2::ThreadWorld<Spec>& thread_world,
  const uitsl::CoarseTimer& run_timer
) {

  std::cout << "thread " << thread_idx;
  std::cout << " @ update " << thread_world.GetUpdate();

  // if limited by updates, print percent updates progress
  if ( dish2::cfg.RUN_UPDATES() ) {
    std::cout << " of " << dish2::cfg.RUN_UPDATES()
      << " ("
      << 100.0 * thread_world.GetUpdate() / dish2::cfg.RUN_UPDATES()
      << "%)";
  }

  std::cout << " + second " << run_timer.GetElapsed().count();

  // if limited by time, print percent time progress
  if ( dish2::cfg.RUN_SECONDS() ) {
    std::cout << " of " << dish2::cfg.RUN_SECONDS()
    << " (" << 100.0 * run_timer.GetFractionComplete() << "%)";
  }

  std::cout << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_PRINT_PROGRESS_HPP_INCLUDE
