#pragma once
#ifndef DISH2_RUN_THREAD_STEP_HPP_INCLUDE
#define DISH2_RUN_THREAD_STEP_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../config/cfg.hpp"

#include "print_progress.hpp"
#include "setup_thread_local_random.hpp"

namespace dish2 {

template<typename Spec>
void thread_step(
  const size_t thread_idx,
  dish2::ThreadWorld<Spec>& thread_world,
  const uitsl::CoarseTimer& run_timer,
  uitsl::CoarseTimer& log_timer
) {

  if ( log_timer.IsComplete() ) {
    log_timer.Reset();
    dish2::print_progress( thread_idx, thread_world, run_timer );
  }

  // update the simulation
  thread_world.Update();


}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_STEP_HPP_INCLUDE
