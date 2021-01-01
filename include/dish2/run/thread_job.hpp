#pragma once
#ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
#define DISH2_RUN_THREAD_JOB_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_audit.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../config/cfg.hpp"
#include "../load/innoculate_population.hpp"
#include "../load/reconstitute_population.hpp"
#include "../world/ThreadWorld.hpp"

#include "thread_data_dump.hpp"
#include "thread_data_write.hpp"
#include "thread_should_continue.hpp"
#include "thread_step.hpp"

namespace dish2 {

template<typename Spec>
void thread_job(
  const size_t thread_idx, dish2::ThreadWorld<Spec> thread_world
) {

  if ( cfg.GENESIS() == "innoculate" )
    dish2::innoculate_population( thread_idx, thread_world );
  else if ( cfg.GENESIS() == "reconstitute" )
    dish2::reconstitute_population( thread_idx, thread_world );
  else emp_always_assert( cfg.GENESIS() == "generate", cfg.GENESIS() );

  const uitsl::CoarseTimer run_timer{
    dish2::cfg.RUN_SECONDS() ?: std::numeric_limits<double>::infinity()
  };

  uitsl::CoarseTimer log_timer{ dish2::cfg.LOG_FREQ() };

  std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " running " << thread_world.population.size() << " cells" << std::endl;

  while ( dish2::thread_should_contine<Spec>( thread_world, run_timer ) ) {
    dish2::thread_step<Spec>( thread_idx, thread_world, run_timer, log_timer );
  }

  std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " simulation complete" << std::endl;

  if ( cfg.RECORD_FINAL_DATA() ) {
    dish2::thread_data_dump<Spec>( thread_world, thread_idx );
    std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " dump complete" << std::endl;

    dish2::thread_data_write<Spec>( thread_world, thread_idx );
    std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " write 0" << std::endl;
    thread_world.Update();
    dish2::thread_data_write<Spec>( thread_world, thread_idx );
    std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " write 1" << std::endl;
  }

  std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " data recorded" << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
