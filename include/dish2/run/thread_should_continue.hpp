#pragma once
#ifndef DISH2_RUN_THREAD_SHOULD_CONTINUE_HPP_INCLUDE
#define DISH2_RUN_THREAD_SHOULD_CONTINUE_HPP_INCLUDE

#include <limits>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../config/cfg.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
bool thread_should_contine(
  const dish2::ThreadWorld<Spec>& thread_world,
  const uitsl::CoarseTimer& run_timer
) {

  // only consider terminating if update is a multiple of 64
  if ( uitsl::shift_mod(thread_world.GetUpdate(), 64) ) return true;
  else return (
    !run_timer.IsComplete()
    && thread_world.GetUpdate()
      < ( dish2::cfg.RUN_UPDATES() ?: std::numeric_limits<size_t>::max() )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_SHOULD_CONTINUE_HPP_INCLUDE
