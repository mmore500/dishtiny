#pragma once
#ifndef DISH2_INTROSPECTION_GET_LIVE_CARDINAL_ROOT_IDS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_LIVE_CARDINAL_ROOT_IDS_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <vector>

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_cardinals.hpp"

namespace dish2 {

// root id of the cell containing each live cardinal
template< typename Spec >
std::vector<size_t> get_live_cardinal_root_ids(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  std::vector<size_t> res;
  res.reserve( dish2::count_cardinals<Spec>( world ) );

  std::for_each(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population ),
    [&]( const auto& cell ){
      std::fill_n(
        std::back_inserter( res ),
        cell.GetNumCardinals(),
        cell.genome->root_id.GetID()
      );
    }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_LIVE_CARDINAL_ROOT_IDS_HPP_INCLUDE
