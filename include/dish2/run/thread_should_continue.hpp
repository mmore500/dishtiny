#pragma once
#ifndef DISH2_RUN_THREAD_SHOULD_CONTINUE_HPP_INCLUDE
#define DISH2_RUN_THREAD_SHOULD_CONTINUE_HPP_INCLUDE

#include <limits>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"

#include "../config/cfg.hpp"
#include "../debug/log_msg.hpp"
#include "../introspection/get_fraction_live_cells.hpp"
#include "../introspection/has_coalesced.hpp"
#include "../introspection/is_extinct.hpp"
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
    dish2::log_msg(
      "coalescence detected at update ", update, ", not continuing"
    );
    return false;
  } else if (
    cfg.ABORT_IF_EXTINCT_FREQ()
    && uitsl::shift_mod(update, cfg.ABORT_IF_EXTINCT_FREQ()) == 0
    && dish2::is_extinct<Spec>( thread_world )
  ) {
    emp_always_assert( !uitsl::is_multiprocess() );
    emp_always_assert( cfg.N_THREADS() == 1 );
    dish2::log_msg(
      "extinction detected at update ", update, ", not continuing"
    );
    return false;
  } else if (
    cfg.ABORT_AT_LIVE_CELL_FRACTION()
    && dish2::get_fraction_live_cells<Spec>( thread_world )
      >= cfg.ABORT_AT_LIVE_CELL_FRACTION()
  ) {
    emp_always_assert( !uitsl::is_multiprocess() );
    emp_always_assert( cfg.N_THREADS() == 1 );
    dish2::log_msg(
      "live cell fraction threshold ", cfg.ABORT_AT_LIVE_CELL_FRACTION(),
      " exceeded at update ", update,
      " with value ", dish2::get_fraction_live_cells<Spec>( thread_world ),
      ", not continuing"
    );
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
