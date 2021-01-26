#pragma once
#ifndef DISH2_RUN_THREAD_STEP_HPP_INCLUDE
#define DISH2_RUN_THREAD_STEP_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/concurrent/ConcurrentBarrier.hpp"
#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/parallel/ThreadIbarrierFactory.hpp"

#include "../config/cfg.hpp"
#include "../record/write_phylogenetic_root_abundances.hpp"

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

  if (
    cfg.ROOT_ABUNDANCES_FREQ()
    && uitsl::shift_mod( thread_world.GetUpdate(), cfg.ROOT_ABUNDANCES_FREQ() )
      == 0
  ) dish2::write_phylogenetic_root_abundances<Spec>( thread_world, thread_idx );

  // update the simulation
  thread_world.Update();

  #ifndef __EMSCRIPTEN__
  if ( cfg.SYNCHRONOUS() ) {

    // initialized first time thru the function,
    // so N_THREADS should be initialized
    static uitsl::ConcurrentBarrier barrier{ cfg.N_THREADS() };

    barrier.ArriveAndWaitWhile( run_timer );

  }
  #endif // #ifndef __EMSCRIPTEN__

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_STEP_HPP_INCLUDE
