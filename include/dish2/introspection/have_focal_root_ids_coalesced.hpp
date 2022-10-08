#pragma once
#ifndef DISH2_INTROSPECTION_HAVE_FOCAL_ROOT_IDS_COALESCED_HPP_INCLUDE
#define DISH2_INTROSPECTION_HAVE_FOCAL_ROOT_IDS_COALESCED_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

#include <algorithm>
#include <iterator>

#include "../cell/Cell.hpp"
#include "../config/is_focal_root_id.hpp"
#include "../genome/RootID.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "no_live_cells_with_focal_root_ids.hpp"

namespace dish2 {

template< typename Spec >
bool have_focal_root_ids_coalesced(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  if ( dish2::no_live_cells_with_focal_root_ids<Spec>( world ) ) return true;
  else {
    using lcit_t = dish2::LiveCellIterator<Spec>;
    using wrapper_t = dish2::RootIDValWrapper<lcit_t>;

    const auto begin = wrapper_t{ lcit_t::make_begin( population ) };
    const auto end = wrapper_t{ lcit_t::make_end( population ) };

    emp::optional<uint64_t> found_root_id;
    for (auto it = begin; it != end; ++it) {
      if (dish2::is_focal_root_id(*it)) {
        if (found_root_id.has_value()) {
          if (*it != *found_root_id) return false;
        } else found_root_id = *it;
      }
    }
    return true;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_HAVE_FOCAL_ROOT_IDS_COALESCED_HPP_INCLUDE
