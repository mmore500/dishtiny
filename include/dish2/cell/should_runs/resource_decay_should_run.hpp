#pragma once
#ifndef DISH2_CELL_SHOULD_RUNS_RESOURCE_DECAY_SHOULD_RUN_HPP_INCLUDE
#define DISH2_CELL_SHOULD_RUNS_RESOURCE_DECAY_SHOULD_RUN_HPP_INCLUDE

#include <cstddef>

namespace dish2 {

constexpr bool resource_decay_should_run(
  const size_t update, const bool alive
) {
  return alive; // TODO paramaterize
}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SHOULD_RUNS_RESOURCE_DECAY_SHOULD_RUN_HPP_INCLUDE
