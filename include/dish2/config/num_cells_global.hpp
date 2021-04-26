#pragma once
#ifndef DISH2_CONFIG_NUM_CELLS_GLOBAL_HPP_INCLUDE
#define DISH2_CONFIG_NUM_CELLS_GLOBAL_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "cfg.hpp"

namespace dish2 {

size_t num_cells_global() {
  return cfg.WEAK_SCALING()
    ? cfg.N_CELLS() * cfg.N_THREADS() * uitsl::get_nprocs()
    : cfg.N_CELLS();
  ;
}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_NUM_CELLS_GLOBAL_HPP_INCLUDE
