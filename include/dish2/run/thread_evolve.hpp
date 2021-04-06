#pragma once
#ifndef DISH2_RUN_THREAD_EVOLVE_HPP_INCLUDE
#define DISH2_RUN_THREAD_EVOLVE_HPP_INCLUDE

#include <fstream>

#include "../../../third-party/conduit/include/uitsl/concurrent/ConcurrentBarrier.hpp"
#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_audit.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"

#include "../config/cfg.hpp"
#include "../debug/log_msg.hpp"
#include "../world/ThreadWorld.hpp"

#include "thread_should_continue.hpp"
#include "thread_step.hpp"

namespace dish2 {

template<typename Spec>
void thread_evolve( dish2::ThreadWorld<Spec>& thread_world ) {

  dish2::log_msg( "running ", thread_world.population.size(), " cells" );

  #ifndef __EMSCRIPTEN__
  // sync before starting job
  static uitsl::ConcurrentBarrier barrier{ cfg.N_THREADS() };
  barrier.ArriveAndWait();
  #endif // #ifndef __EMSCRIPTEN__

  const uitsl::CoarseTimer run_timer{
    dish2::cfg.RUN_SECONDS() ?: std::numeric_limits<double>::infinity()
  };

  uitsl::CoarseTimer log_timer{ dish2::cfg.LOG_FREQ() };

  while ( dish2::thread_should_contine<Spec>( thread_world, run_timer ) ) {
    dish2::thread_step<Spec>( thread_world, run_timer, log_timer );
  }

  dish2::log_msg(
    "simulation complete @ ", thread_world.GetUpdate(), " updates"
  );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_EVOLVE_HPP_INCLUDE
