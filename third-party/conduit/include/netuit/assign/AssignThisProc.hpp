#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNTHISPROC_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNTHISPROC_HPP_INCLUDE

#include "../../uitsl/mpi/mpi_utils.hpp"

namespace netuit {

struct AssignThisProc {

  uitsl::proc_id_t operator()(const size_t & node_id) {
    return uitsl::get_proc_id();
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNTHISPROC_HPP_INCLUDE
