#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_UNIQUE_ROOT_IDS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_UNIQUE_ROOT_IDS_HPP_INCLUDE

#include <iterator>
#include <set>
#include <type_traits>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/RootIDConstWrapper.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t count_unique_root_ids( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  using wrapper_t = dish2::RootIDConstWrapper<
    dish2::LiveCellIterator<Spec>
  >;

  return std::set< std::remove_const_t<typename wrapper_t::value_type> >(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) }
  ).size();

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_UNIQUE_ROOT_IDS_HPP_INCLUDE
