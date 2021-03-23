#pragma once
#ifndef DISH2_INTROSPECTION_GET_ROOT_ID_COUNTS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_ROOT_ID_COUNTS_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <unordered_map>

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/RootIDValWrapper.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
std::unordered_map<size_t, size_t>
get_root_id_counts( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  using wrapper_t = dish2::RootIDValWrapper<
    dish2::LiveCellIterator<Spec>
  >;

  std::unordered_map<size_t, size_t> res;

  std::for_each(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) },
    [&res]( const size_t root_id ){ ++res[ root_id ]; }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_ROOT_ID_COUNTS_HPP_INCLUDE
