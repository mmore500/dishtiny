#pragma once
#ifndef DISH2_CELL_SHOULD_RUNS_RESOURCE_RECEIVING_SHOULD_RUN_HPP_INCLUDE
#define DISH2_CELL_SHOULD_RUNS_RESOURCE_RECEIVING_SHOULD_RUN_HPP_INCLUDE

#include <cstddef>
#include <limits>

#include "../../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../../config/cfg.hpp"

namespace dish2 {

bool resource_receiving_should_run( const size_t update, const bool alive ) {
  const size_t freq = dish2::cfg.RESOURCE_RECEIVING_SERVICE_FREQUENCY();
  return
    alive
    && freq > 0
    && uitsl::shift_mod( update, freq ) == 0;
}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SHOULD_RUNS_RESOURCE_RECEIVING_SHOULD_RUN_HPP_INCLUDE
