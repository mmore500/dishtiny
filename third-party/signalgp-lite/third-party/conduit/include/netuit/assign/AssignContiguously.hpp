#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNCONTIGUOUSLY_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNCONTIGUOUSLY_HPP_INCLUDE

#include <functional>
#include <stddef.h>

namespace netuit {

template<typename RETURN_TYPE>
struct AssignContiguously {

  const size_t num_threads;
  const size_t num_nodes;

  AssignContiguously(
    const size_t num_threads_,
    const size_t num_nodes_
  ) : num_threads(num_threads_)
  , num_nodes(num_nodes_)
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return (node_id * num_threads / num_nodes) % num_threads;
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNCONTIGUOUSLY_HPP_INCLUDE
