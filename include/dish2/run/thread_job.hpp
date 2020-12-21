#pragma once
#ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
#define DISH2_RUN_THREAD_JOB_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_audit.hpp"

#include "../config/cfg.hpp"
#include "../load/innoculate_population.hpp"
#include "../load/reconstitute_population.hpp"
#include "../record/dump_abundance_genome.hpp"
#include "../record/dump_arbitrary_genome.hpp"
#include "../record/dump_population.hpp"
#include "../record/write_demographic_phenotypic_phylogenetic_metrics.hpp"
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

  if ( cfg.GENESIS() == "innoculate" )
    dish2::innoculate_population( thread_idx, thread_world );
  else if ( cfg.GENESIS() == "reconstitute" )
    dish2::reconstitute_population( thread_idx, thread_world );
  else emp_always_assert( cfg.GENESIS() == "generate", cfg.GENESIS() );

  const uitsl::CoarseTimer run_timer{
    dish2::cfg.RUN_SECONDS() ?: std::numeric_limits<double>::infinity()
  };

  uitsl::CoarseTimer log_timer{ dish2::cfg.LOG_FREQ() };

  while ( dish2::thread_should_contine<Spec>( thread_world, run_timer ) ) {
    dish2::thread_step<Spec>( thread_idx, thread_world, run_timer, log_timer );
  }

  std::cout << "thread " << thread_idx << " simulation complete" << std::endl;

  uitsl::err_audit(!
    dish2::dump_abundance_genome<Spec>( thread_world, thread_idx )
  );
  uitsl::err_audit(!
    dish2::dump_arbitrary_genome<Spec>( thread_world, thread_idx )
  );
  dish2::dump_population<Spec>( thread_world, thread_idx );
  dish2::write_demographic_phenotypic_phylogenetic_metrics<Spec>(
    thread_world, thread_idx
  );

  std::cout << "thread " << thread_idx << " data dump complete" << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
