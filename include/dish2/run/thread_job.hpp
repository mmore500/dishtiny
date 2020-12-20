#pragma once
#ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
#define DISH2_RUN_THREAD_JOB_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../config/cfg.hpp"
#include "../introspection/write_summary_metrics.hpp"
#include "../world/ThreadWorld.hpp"

#include "setup_thread_local_random.hpp"
#include "thread_should_continue.hpp"
#include "thread_step.hpp"

namespace dish2 {

template<typename Spec>
void thread_job(
  const size_t thread_idx, dish2::ThreadWorld<Spec> thread_world
) {

  dish2::setup_thread_local_random( thread_idx );

  const uitsl::CoarseTimer run_timer{
    dish2::cfg.RUN_SECONDS() ?: std::numeric_limits<double>::infinity()
  };

  uitsl::CoarseTimer log_timer{ dish2::cfg.LOG_FREQ() };

  while ( dish2::thread_should_contine<Spec>( thread_world, run_timer ) ) {
    dish2::thread_step<Spec>( thread_idx, thread_world, run_timer, log_timer );
  }

  dish2::write_summary_metrics<Spec>( thread_world, thread_idx );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
