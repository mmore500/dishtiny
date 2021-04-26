#pragma once
#ifndef DISH2_INTROSPECTION_GET_KIN_GROUP_SIZES_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_KIN_GROUP_SIZES_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <unordered_map>

#include "../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/WorldIteratorAbridger.hpp"
#include "../world/ThreadWorld.hpp"

#include "no_live_cells.hpp"

namespace dish2 {

/// @return map of kin group IDs to counts.
template< typename Spec >
auto get_kin_group_sizes(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  using iterator_t = dish2::WorldIteratorAbridger<
    lcit_t,
    dish2::KinGroupIDViewWrapper<Spec>
  >;


  std::unordered_map<size_t, size_t> res;

  if ( dish2::no_live_cells<Spec>( world ) ) return res;

  std::for_each(
    iterator_t{ lcit_t::make_begin( population ) },
    iterator_t{ lcit_t::make_end( population ) },
    [&res, lev]( const auto& kin_group_id_view ) {
      const auto kin_group_id = kin_group_id_view.GetBuffer()[ lev ];
      ++res[ kin_group_id ];
    }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_KIN_GROUP_SIZES_HPP_INCLUDE
