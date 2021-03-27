#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_RESOURCE_RECEIVED_PER_CELL_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_RESOURCE_RECEIVED_PER_CELL_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <limits>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../cell/cardinal_iterators/ResourceInputPeekWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/WorldIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_live_cells.hpp"
#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_mean_resource_received_per_cell(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  using iterator_t = dish2::WorldIteratorAdapter<
    lcit_t,
    dish2::ResourceInputPeekWrapper<Spec>
  >;

  emp_assert( dish2::no_live_cells<Spec>( world ) || std::none_of(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) ),
    []( const auto val ){ return std::isnan(val); }
  ) );

  if ( dish2::no_live_cells<Spec>( world ) ) {
    return std::numeric_limits<double>::quiet_NaN();
  } else return std::accumulate(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) ),
    double{}
  ) / dish2::count_live_cells< Spec >( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_RESOURCE_RECEIVED_PER_CELL_HPP_INCLUDE
