#pragma once
#ifndef DISH2_INTROSPECTION_GET_FRACTION_CELLS_KIN_GROUP_LONERS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_FRACTION_CELLS_KIN_GROUP_LONERS_HPP_INCLUDE

#include "count_live_cells.hpp"
#include "get_num_kin_group_loners.hpp"

namespace dish2 {

template< typename Spec >
double get_fraction_cells_kin_group_loners(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  return dish2::get_num_kin_group_loners<Spec>( world, lev )
    / static_cast<double>( dish2::count_live_cells( world ) );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_FRACTION_CELLS_KIN_GROUP_LONERS_HPP_INCLUDE
