#pragma once
#ifndef DISH2_VIZ_GETTERS_DISTANCETOGRAPHCENTERGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_DISTANCETOGRAPHCENTERGETTER_HPP_INCLUDE

#include <functional>
#include <tuple>

#include "../../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../../cell/Cell.hpp"
#include "../../push/DistanceToGraphCenterCellState.hpp"
#include "../../push/PushCellState.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
class DistanceToGraphCenterGetter {

  std::reference_wrapper<const dish2::ThreadWorld<Spec>> thread_world;

public:

  using value_type = emp::optional<size_t>;

  template< typename... Args >
  DistanceToGraphCenterGetter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    Args&&...
  )
  : thread_world(thread_world_)
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    return std::get< dish2::DistanceToGraphCenterCellState >(
      thread_world.get().GetCell( cell_idx ).cell_push_state
    ).my_distance;
  }

  size_t GetNumCells() const { return thread_world.get().GetSize(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return thread_world.get().GetCell( cell_idx ).GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_DISTANCETOGRAPHCENTERGETTER_HPP_INCLUDE
