#pragma once
#ifndef DISH2_INTROSPECTION_IS_EXTINCT_HPP_INCLUDE
#define DISH2_INTROSPECTION_IS_EXTINCT_HPP_INCLUDE

#include "../world/ThreadWorld.hpp"

#include "any_live_cells.hpp"

namespace dish2 {

template< typename Spec >
bool is_extinct( const dish2::ThreadWorld<Spec>& world ) {

  return !dish2::any_live_cells<Spec>( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_IS_EXTINCT_HPP_INCLUDE
