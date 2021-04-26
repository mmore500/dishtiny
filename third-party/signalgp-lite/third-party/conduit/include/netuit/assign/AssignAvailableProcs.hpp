#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNAVAILABLEPROCS_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNAVAILABLEPROCS_HPP_INCLUDE

#include "../../uitsl/mpi/mpi_utils.hpp"

namespace netuit {

struct AssignAvailableProcs {

  uitsl::proc_id_t operator()(const size_t & node_id) {
    return node_id % uitsl::get_nprocs();
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNAVAILABLEPROCS_HPP_INCLUDE
