#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNSEGREGATED_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNSEGREGATED_HPP_INCLUDE

#include <functional>
#include <stddef.h>

namespace netuit {

template<typename RETURN_TYPE>
struct AssignSegregated {

  RETURN_TYPE operator()(const size_t & node_id) {
    return node_id;
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNSEGREGATED_HPP_INCLUDE
