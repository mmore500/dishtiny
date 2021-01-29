#pragma once
#ifndef DISH2_INTROSPECTION_NO_LIVE_CELLS_HPP_INCLUDE
#define DISH2_INTROSPECTION_NO_LIVE_CELLS_HPP_INCLUDE

#include "any_live_cells.hpp"

namespace dish2 {

template< typename Spec >
size_t no_live_cells( const dish2::ThreadWorld<Spec>& world ) {

  return !any_live_cells<Spec>( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_NO_LIVE_CELLS_HPP_INCLUDE
