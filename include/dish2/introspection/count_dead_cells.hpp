#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_DEAD_CELLS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_DEAD_CELLS_HPP_INCLUDE

#include <iterator>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_live_cells.hpp"

namespace dish2 {

template< typename Spec >
size_t count_dead_cells( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  return population.size() - dish2::count_live_cells( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_DEAD_CELLS_HPP_INCLUDE
