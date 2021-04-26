#pragma once
#ifndef DISH2_INTROSPECTION_ANY_LIVE_CELLS_HPP_INCLUDE
#define DISH2_INTROSPECTION_ANY_LIVE_CELLS_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_live_cells.hpp"

namespace dish2 {

template< typename Spec >
bool any_live_cells( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  const bool res = (
    dish2::LiveCellIterator<Spec>::make_begin( population )
    != dish2::LiveCellIterator<Spec>::make_end( population )
  );

  emp_assert(
    res == ( dish2::count_live_cells<Spec>( world ) > 0 )
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_ANY_LIVE_CELLS_HPP_INCLUDE
