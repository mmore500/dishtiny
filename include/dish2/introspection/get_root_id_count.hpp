#pragma once
#ifndef DISH2_INTROSPECTION_GET_ROOT_ID_COUNT_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_ROOT_ID_COUNT_HPP_INCLUDE

#include <iterator>
#include <limits>
#include <set>
#include <type_traits>

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/RootIDValWrapper.hpp"
#include "../world/ThreadWorld.hpp"

#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_root_id_count(
  const size_t root_id, const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;
  using wrapper_t = dish2::RootIDValWrapper<lcit_t>;

  if ( dish2::no_live_cells<Spec>( world ) ) return 0;
  else return std::count(
    wrapper_t{ lcit_t::make_begin( population ) },
    wrapper_t{ lcit_t::make_end( population ) },
    root_id
  );


}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_ROOT_ID_COUNT_HPP_INCLUDE
