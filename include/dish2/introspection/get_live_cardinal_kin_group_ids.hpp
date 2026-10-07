#pragma once
#ifndef DISH2_INTROSPECTION_GET_LIVE_CARDINAL_KIN_GROUP_IDS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_LIVE_CARDINAL_KIN_GROUP_IDS_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <vector>

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_cardinals.hpp"

namespace dish2 {

// kin group id at the given level of the cell containing each live cardinal
template< typename Spec >
std::vector<size_t> get_live_cardinal_kin_group_ids(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
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
        cell.genome->kin_group_id.GetBuffer()[ lev ]
      );
    }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_LIVE_CARDINAL_KIN_GROUP_IDS_HPP_INCLUDE
