#pragma once
#ifndef DISH2_INTROSPECTION_GET_FRACTION_LIVE_CELLS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_FRACTION_LIVE_CELLS_HPP_INCLUDE

#include "../world/ThreadWorld.hpp"

#include "count_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_fraction_live_cells( const dish2::ThreadWorld<Spec>& world ) {

  return dish2::count_live_cells<Spec>(
    world
  ) / static_cast<double>( world.GetSize() );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_FRACTION_LIVE_CELLS_HPP_INCLUDE
