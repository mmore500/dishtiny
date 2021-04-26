#pragma once
#ifndef DISH2_RUNNINGLOG_SPAWNEVENT_HPP_INCLUDE
#define DISH2_RUNNINGLOG_SPAWNEVENT_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/array.hpp"

namespace dish2 {

template< typename Spec >
struct SpawnEvent {

  size_t kin_id_commonality_daughter_eliminated;
  size_t kin_id_commonality_parent_daughter;
  size_t kin_id_commonality_parent_eliminated;
  size_t num_neighbors_parent;
  emp::array<size_t, Spec::NLEV> peripherality_parent;
  size_t replev;

};

} // namespace dish2



#endif // #ifndef DISH2_RUNNINGLOG_SPAWNEVENT_HPP_INCLUDE
