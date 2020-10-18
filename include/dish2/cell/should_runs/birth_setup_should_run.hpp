#pragma once
#ifndef DISH2_CELL_SHOULD_RUNS_BIRTH_SETUP_SHOULD_RUN_HPP_INCLUDE
#define DISH2_CELL_SHOULD_RUNS_BIRTH_SETUP_SHOULD_RUN_HPP_INCLUDE

#include <cstddef>
#include <limits>

#include "../../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../../config/cfg.hpp"

namespace dish2 {

bool birth_setup_should_run( const size_t update, const bool ) {
  const size_t freq = dish2::cfg.BIRTH_SETUP_SERVICE_FREQUENCY();
  return
    freq > 0
    && uitsl::shift_mod( update, freq ) == 0;
}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SHOULD_RUNS_BIRTH_SETUP_SHOULD_RUN_HPP_INCLUDE
