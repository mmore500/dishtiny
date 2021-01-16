#pragma once
#ifndef DISH2_RUN_THREAD_DATA_DUMP_HPP_INCLUDE
#define DISH2_RUN_THREAD_DATA_DUMP_HPP_INCLUDE

#include <limits>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../config/cfg.hpp"
#include "../record/dump_abundance_genome.hpp"
#include "../record/dump_arbitrary_genome.hpp"
#include "../record/dump_population.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

// data collection tasks that must be run only once
template<typename Spec>
void thread_data_dump(
  const dish2::ThreadWorld<Spec>& thread_world,
  const size_t thread_idx
) {

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
