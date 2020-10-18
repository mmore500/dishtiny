#pragma once
#ifndef DISH2_CELL_SHOULD_RUNS_EVENT_LAUNCHING_SHOULD_RUN_HPP_INCLUDE
#define DISH2_CELL_SHOULD_RUNS_EVENT_LAUNCHING_SHOULD_RUN_HPP_INCLUDE

#include <cstddef>

namespace dish2 {

constexpr bool event_launching_should_run(
  const size_t update, const bool alive
) {
  return alive; // TODO paramaterize
}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SHOULD_RUNS_EVENT_LAUNCHING_SHOULD_RUN_HPP_INCLUDE
