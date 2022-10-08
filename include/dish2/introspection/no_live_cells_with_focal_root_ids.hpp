#pragma once
#ifndef DISH2_INTROSPECTION_NO_LIVE_CELLS_WITH_FOCAL_ROOT_IDS_HPP_INCLUDE
#define DISH2_INTROSPECTION_NO_LIVE_CELLS_WITH_FOCAL_ROOT_IDS_HPP_INCLUDE

#include "any_live_cells_with_focal_root_ids.hpp"

namespace dish2 {

template< typename Spec >
size_t no_live_cells_with_focal_root_ids(
  const dish2::ThreadWorld<Spec>& world
) {

  return !dish2::any_live_cells_with_focal_root_ids<Spec>( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_NO_LIVE_CELLS_WITH_FOCAL_ROOT_IDS_HPP_INCLUDE
