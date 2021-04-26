#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNAVAILABLETHREADS_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNAVAILABLETHREADS_HPP_INCLUDE

#include "../../uitsl/parallel/thread_utils.hpp"

namespace netuit {

struct AssignAvailableThreads {

  uitsl::thread_id_t operator()(const size_t & node_id) {
    return node_id % uitsl::get_nproc();
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNAVAILABLETHREADS_HPP_INCLUDE
