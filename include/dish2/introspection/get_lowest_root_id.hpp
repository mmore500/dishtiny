#pragma once
#ifndef DISH2_INTROSPECTION_GET_LOWEST_ROOT_ID_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_LOWEST_ROOT_ID_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/RootIDValWrapper.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t get_lowest_root_id( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  using wrapper_t = dish2::RootIDValWrapper<
    dish2::LiveCellIterator<Spec>
  >;

  return *std::min_element(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_LOWEST_ROOT_ID_HPP_INCLUDE
