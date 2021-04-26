#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_LIVE_CELLS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_LIVE_CELLS_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t count_live_cells( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  const auto res = std::count_if(
    std::begin( population ),
    std::end( population ),
    []( const auto& cell ){ return cell.IsAlive(); }
  );

  emp_assert(res == std::distance(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population )
  ) );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_LIVE_CELLS_HPP_INCLUDE
