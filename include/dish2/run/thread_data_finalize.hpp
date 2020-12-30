#pragma once
#ifndef DISH2_RUN_THREAD_DATA_FINALIZE_HPP_INCLUDE
#define DISH2_RUN_THREAD_DATA_FINALIZE_HPP_INCLUDE

#include <limits>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../config/cfg.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

// data collection tasks that must be run only once
template<typename Spec>
void thread_data_finalize(
  const dish2::ThreadWorld<Spec>& thread_world,
  const size_t thread_idx
) {

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_DATA_FINALIZE_HPP_INCLUDE
