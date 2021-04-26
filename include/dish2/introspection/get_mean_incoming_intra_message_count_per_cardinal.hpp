#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_INCOMING_INTRA_MESSAGE_COUNT_PER_CARDINAL_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_INCOMING_INTRA_MESSAGE_COUNT_PER_CARDINAL_HPP_INCLUDE

#include <algorithm>
#include <limits>

#include "../cell/cardinal_iterators/IncomingIntraMessageCounterWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/WorldIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_live_cardinals.hpp"
#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_mean_incoming_intra_message_count_per_cardinal(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  using iterator_t = dish2::WorldIteratorAdapter<
    lcit_t,
    dish2::IncomingIntraMessageCounterWrapper<Spec>
  >;

  if ( dish2::no_live_cells<Spec>( world ) ) {
    return std::numeric_limits<double>::quiet_NaN();
  } else return std::accumulate(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) ),
    double{}
  ) / dish2::count_live_cardinals< Spec >( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_INCOMING_INTRA_MESSAGE_COUNT_PER_CARDINAL_HPP_INCLUDE
