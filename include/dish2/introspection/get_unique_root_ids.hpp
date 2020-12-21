#pragma once
#ifndef DISH2_INTROSPECTION_GET_UNIQUE_ROOT_IDS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_UNIQUE_ROOT_IDS_HPP_INCLUDE

#include <iterator>
#include <set>

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/RootIDValWrapper.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
std::set<size_t> get_unique_root_ids( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  using wrapper_t = dish2::RootIDValWrapper<
    dish2::LiveCellIterator<Spec>
  >;

  return std::set< size_t >(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_UNIQUE_ROOT_IDS_HPP_INCLUDE
