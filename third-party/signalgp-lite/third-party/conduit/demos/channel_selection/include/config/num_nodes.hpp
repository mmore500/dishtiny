#pragma once

#include "uitsl/mpi/comm_utils.hpp"

#include "cfg.hpp"

size_t num_nodes() {
  return  uitsl::get_nprocs() * cfg.N_THREADS() * cfg.N_NODES_PER_CPU();
}
