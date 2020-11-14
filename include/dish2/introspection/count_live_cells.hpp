#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_LIVE_CELLS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_LIVE_CELLS_HPP_INCLUDE

#include <iterator>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t count_live_cells( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  return std::distance(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_LIVE_CELLS_HPP_INCLUDE
