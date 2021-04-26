#pragma once
#ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
#define DISH2_RUN_THREAD_JOB_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../config/cfg.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../load/load_world.hpp"
#include "../record/dump_coalescence_result.hpp"
#include "../record/dump_interroot_phenotype_differentiation.hpp"
#include "../world/ThreadWorld.hpp"

#include "thread_artifacts_dump.hpp"
#include "thread_benchmarks_dump.hpp"
#include "thread_data_dump.hpp"
#include "thread_data_write.hpp"
#include "thread_evolve.hpp"
#include "thread_try_end_snapshot.hpp"

namespace dish2 {

template<typename Spec>
void thread_job( dish2::ThreadWorld<Spec> thread_world ) {

  dish2::load_world<Spec>( thread_world );

  if ( cfg.RUN() ) dish2::thread_evolve<Spec>( thread_world );

  if ( cfg.TEST_INTERROOT_PHENOTYPE_DIFFERENTIATION() ) {
    dish2::dump_interroot_phenotype_differentiation( thread_world );
  }

  // write elapsed updates to file (for easier benchmark post-processing)
  if (cfg.BENCHMARKING_DUMP() ) {
    dish2::thread_benchmarks_dump<Spec>( thread_world );
    dish2::log_msg( "benchmarks dump complete" );

  }

  if ( cfg.ARTIFACTS_DUMP() ) {
    dish2::thread_artifacts_dump<Spec>( thread_world );
    dish2::log_msg( "artifacts dump complete" );
  }

  if (dish2::cfg.GENESIS() == "innoculate") {
    dish2::dump_coalescence_result<Spec>( thread_world );
  }

  dish2::thread_try_end_snapshot<Spec>( thread_world );

  dish2::log_msg( "thread job complete" );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
