#pragma once
#ifndef DISH2_CELL_SHOULD_RUNS_STATE_INPUT_JUMP_SHOULD_RUN_HPP_INCLUDE
#define DISH2_CELL_SHOULD_RUNS_STATE_INPUT_JUMP_SHOULD_RUN_HPP_INCLUDE

#include <cstddef>
#include <limits>

#include "../../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../../config/cfg.hpp"

namespace dish2 {

bool state_input_jump_should_run(const size_t update, const bool alive) {
  const size_t freq = dish2::cfg.STATE_INPUT_JUMP_FREQUENCY();
  return
    freq > 0
    && uitsl::shift_mod( update, freq ) == 0;
}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SHOULD_RUNS_STATE_INPUT_JUMP_SHOULD_RUN_HPP_INCLUDE
