#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_KIN_NEIGHBORS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_KIN_NEIGHBORS_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t count_kin_neighbors(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  const auto& population = world.population;

  return std::accumulate(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population ),
    size_t{},
    [lev]( const size_t accumulator, const auto& cell ) {
      emp_assert( cell.GetNumCardinals() >= cell.GetPeripherality( lev ) );
      const size_t num_kin_neighbors
        = cell.GetNumCardinals() - cell.GetPeripherality( lev );
      return accumulator + num_kin_neighbors;
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_KIN_NEIGHBORS_HPP_INCLUDE
