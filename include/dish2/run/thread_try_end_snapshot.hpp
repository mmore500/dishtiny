#pragma once
#ifndef DISH2_RUN_THREAD_TRY_END_SNAPSHOT_HPP_INCLUDE
#define DISH2_RUN_THREAD_TRY_END_SNAPSHOT_HPP_INCLUDE

#include <chrono>
#include <iostream>

#include "../config/cfg.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../utility/try_with_timeout.hpp"
#include "../world/ThreadWorld.hpp"

#include "thread_end_snapshot.hpp"

namespace dish2 {

template<typename Spec>
void thread_try_end_snapshot( dish2::ThreadWorld<Spec>& thread_world ) {

  if ( !cfg.DATA_DUMP() ) {
    dish2::log_msg( "skipping end snapshot" );
    return;
  }

  const bool res = dish2::try_with_timeout(
    [&](){ dish2::thread_end_snapshot<Spec>(thread_world); },
    std::chrono::duration<double>(
      dish2::cfg.END_SNAPSHOT_TIMEOUT_SECONDS()
    )
  );

  if ( res ) dish2::log_msg( "end snapshot success" );
  else dish2::log_msg( "end snapshot timed out" );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_TRY_END_SNAPSHOT_HPP_INCLUDE
