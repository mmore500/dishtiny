#pragma once
#ifndef DISH2_RUN_THREAD_DATA_DUMP_HPP_INCLUDE
#define DISH2_RUN_THREAD_DATA_DUMP_HPP_INCLUDE

#include <limits>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../config/cfg.hpp"
#include "../record/dump_abundance_genome.hpp"
#include "../record/dump_arbitrary_genome.hpp"
#include "../record/dump_birth_log.hpp"
#include "../record/dump_coalescence_result.hpp"
#include "../record/dump_death_log.hpp"
#include "../record/dump_kin_conflict_by_replev_statistics.hpp"
#include "../record/dump_kin_conflict_statistics.hpp"
#include "../record/dump_population.hpp"
#include "../record/dump_spawn_log.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

// data collection tasks that must be run only once
template<typename Spec>
void thread_data_dump(
  const dish2::ThreadWorld<Spec>& thread_world,
  const size_t thread_idx
) {


  if (dish2::cfg.GENESIS() == "innoculate") {
    dish2::dump_coalescence_result<Spec>( thread_world, thread_idx );
  }
  dish2::dump_kin_conflict_by_replev_statistics<Spec>(
    thread_world, thread_idx
  );
  dish2::dump_kin_conflict_statistics<Spec>(
    thread_world, thread_idx
  );
  dish2::dump_birth_log<Spec>( thread_world, thread_idx );
  dish2::dump_death_log<Spec>( thread_world, thread_idx );
  dish2::dump_spawn_log<Spec>( thread_world, thread_idx );

  uitsl_err_audit(!
    dish2::dump_abundance_genome<Spec>( thread_world, thread_idx )
  );
  uitsl_err_audit(!
    dish2::dump_arbitrary_genome<Spec>( thread_world, thread_idx )
  );
  dish2::dump_population<Spec>( thread_world, thread_idx );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_DATA_DUMP_HPP_INCLUDE
