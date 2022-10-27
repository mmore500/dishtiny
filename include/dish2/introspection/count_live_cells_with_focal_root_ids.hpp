#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_LIVE_CELLS_WITH_FOCAL_ROOT_IDS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_LIVE_CELLS_WITH_FOCAL_ROOT_IDS_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../config/is_focal_root_id.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t count_live_cells_with_focal_root_ids(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  const auto res = std::count_if(
    std::begin( population ),
    std::end( population ),
    []( const auto& cell ){
      return cell.IsAlive() && dish2::is_focal_root_id(
        cell.GetGenome().root_id
      );
    }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_LIVE_CELLS_WITH_FOCAL_ROOT_IDS_HPP_INCLUDE
