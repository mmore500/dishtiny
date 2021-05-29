#pragma once
#ifndef DISH2_RUN_THREAD_STEP_HPP_INCLUDE
#define DISH2_RUN_THREAD_STEP_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/chrono/ClockDeltaDetector.hpp"
#include "../../../third-party/conduit/include/uitsl/concurrent/ConcurrentBarrier.hpp"
#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../config/cfg.hpp"
#include "../record/write_phylogenetic_root_abundances.hpp"
#include "../record/write_selected_frames.hpp"

#include "print_progress.hpp"
#include "setup_thread_local_random.hpp"

namespace dish2 {

template<typename Spec>
void thread_step(
  dish2::ThreadWorld<Spec>& thread_world,
  const uitsl::CoarseTimer& run_timer,
  uitsl::CoarseTimer& log_timer
) {

  if ( log_timer.IsComplete() ) {
    log_timer.Reset();
    dish2::print_progress( thread_world, run_timer );
  }

  if (
    cfg.ROOT_ABUNDANCES_FREQ()
    && uitsl::shift_mod( thread_world.GetUpdate(), cfg.ROOT_ABUNDANCES_FREQ() )
      == 0
  ) dish2::write_phylogenetic_root_abundances<Spec>( thread_world );

  // update the simulation
  thread_world.Update();

  if (
    dish2::cfg.SELECTED_DRAWINGS_FREQ()
    && uitsl::shift_mod(
      thread_world.GetUpdate(), dish2::cfg.SELECTED_DRAWINGS_FREQ()
    ) == 0
    && dish2::cfg.SELECTED_DRAWINGS().size()
  ) dish2::write_selected_frames<Spec>( thread_world );

  #ifndef __EMSCRIPTEN__
  // initialized first time thru the function,
  // so N_THREADS should be initialized
  static uitsl::ConcurrentBarrier barrier{ cfg.N_THREADS() };

  if ( !cfg.ASYNCHRONOUS() ) {
    barrier.ArriveAndWaitWhile( run_timer );
  } else if (
    thread_local uitsl::CoarseRealTimer timer_sync{
      std::chrono::milliseconds{ cfg.SYNC_FREQ_MILLISECONDS() }
    };
    uitsl::is_multiprocess()
      && cfg.ASYNCHRONOUS() == 1
      && timer_sync.IsComplete()
  ) {
    barrier.ArriveAndWaitWhile( run_timer );
    timer_sync.Reset();
  } else if (
    thread_local uitsl::ClockDeltaDetector<> delta_sync;
    uitsl::is_multiprocess()
      && cfg.ASYNCHRONOUS() == 2
      && delta_sync.HasDeltaElapsed()
  ) {
    barrier.ArriveAndWaitWhile( run_timer );
  } else {
    // nop for cfg.ASYNCHRONOUS() == 3
  }

  #endif // #ifndef __EMSCRIPTEN__

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_STEP_HPP_INCLUDE
