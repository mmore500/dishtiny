#pragma once
#ifndef DISH2_VIZ_GETTERS_NEIGHBORPOSGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_NEIGHBORPOSGETTER_HPP_INCLUDE

#include <functional>

#include "../../cell/Cell.hpp"
#include "../../config/cfg.hpp"
#include "../../genome/Genome.hpp"
#include "../../genome/RootID.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
class NeighborPosGetter {

  std::reference_wrapper<const dish2::ThreadWorld<Spec>> thread_world;

public:

  using value_type = size_t;

  template< typename... Args >
  NeighborPosGetter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    Args&&...
  )
  : thread_world(thread_world_)
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {

    const size_t cardinality = GetNumCells();
    const size_t dimension = std::sqrt( cardinality );
    const size_t first_idx_in_row = (cell_idx / dimension) * dimension;
    const size_t idx_in_row = cell_idx % dimension;

    switch (cardinal_idx) {
      case 3:
        // north
        return (cell_idx + cardinality - dimension) % cardinality;
      case 2:
        // west
        return first_idx_in_row + (idx_in_row + dimension - 1) % dimension;
      case 1:
        // east
        return first_idx_in_row + (idx_in_row + 1) % dimension;
      case 0:
        // south
        return (cell_idx + dimension) % cardinality;
      default:
        emp_assert( false, cardinal_idx );
        __builtin_unreachable();
    }

  }

  size_t GetNumCells() const { return thread_world.get().GetSize(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return thread_world.get().GetCell( cell_idx ).GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_NEIGHBORPOSGETTER_HPP_INCLUDE
