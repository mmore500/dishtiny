#pragma once
#ifndef DISH2_INTROSPECTION_HAS_COALESCED_HPP_INCLUDE
#define DISH2_INTROSPECTION_HAS_COALESCED_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
bool has_coalesced( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  if ( dish2::no_live_cells<Spec>( world ) ) return true;
  else {
    const auto begin = dish2::LiveCellIterator<Spec>::make_begin(
      population
    );
    const auto end = dish2::LiveCellIterator<Spec>::make_end(
      population
    );

    return std::adjacent_find(
      begin, end,
      [](const auto& left_cell, const auto& right_cell){
        return left_cell.genome->root_id != right_cell.genome->root_id;
      }
    ) == end;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_HAS_COALESCED_HPP_INCLUDE
