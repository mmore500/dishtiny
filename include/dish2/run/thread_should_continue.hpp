#pragma once
#ifndef DISH2_RUN_THREAD_SHOULD_CONTINUE_HPP_INCLUDE
#define DISH2_RUN_THREAD_SHOULD_CONTINUE_HPP_INCLUDE

#include <limits>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"

#include "../config/cfg.hpp"
#include "../introspection/has_coalesced.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
bool thread_should_contine(
  const dish2::ThreadWorld<Spec>& thread_world,
  const uitsl::CoarseTimer& run_timer
) {

  const size_t update = thread_world.GetUpdate();

  if (
    cfg.ABORT_IF_COALESCENT_FREQ()
    && uitsl::shift_mod(update, cfg.ABORT_IF_COALESCENT_FREQ()) == 0
    && dish2::has_coalesced<Spec>( thread_world )
  ) {
    emp_always_assert( !uitsl::is_multiprocess() );
    emp_always_assert( cfg.N_THREADS() == 1 );
    return false;
  } else if (
    // if we are going to dump data,
    // only consider terminating if update is a multiple of 64, minus one
    uitsl::shift_mod(update, 64) != 63
    && cfg.DATA_DUMP()
  ) return true;
  else return (
    !run_timer.IsComplete()
    && update < (dish2::cfg.RUN_UPDATES() ?: std::numeric_limits<size_t>::max())
  );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_SHOULD_CONTINUE_HPP_INCLUDE
