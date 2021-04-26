#pragma once
#ifndef DISH2_RUN_PRINT_PROGRESS_HPP_INCLUDE
#define DISH2_RUN_PRINT_PROGRESS_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void print_progress(
  const dish2::ThreadWorld<Spec>& thread_world,
  const uitsl::CoarseTimer& run_timer
) {

  const size_t update = thread_world.GetUpdate();

  if ( dish2::cfg.RUN_UPDATES() ) {
    dish2::log_msg(
      "update ", update, " of ", dish2::cfg.RUN_UPDATES(),
      " (", 100.0 * update / dish2::cfg.RUN_UPDATES(), "%)"
    );
  } else dish2::log_msg( "update ", update );


  if ( dish2::cfg.RUN_SECONDS() ) {
    dish2::log_msg(
      "second ", run_timer.GetElapsed().count(),
      " of ", dish2::cfg.RUN_SECONDS(),
      " (", 100.0 * run_timer.GetFractionComplete(), "%)"
    );
  } else dish2::log_msg( "second ", run_timer.GetElapsed().count() );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_PRINT_PROGRESS_HPP_INCLUDE
