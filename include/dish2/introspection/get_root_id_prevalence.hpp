#pragma once
#ifndef DISH2_INTROSPECTION_GET_ROOT_ID_PREVALENCE_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_ROOT_ID_PREVALENCE_HPP_INCLUDE

#include <iterator>
#include <set>
#include <type_traits>

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/RootIDValWrapper.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
double get_root_id_prevalence(
  const size_t root_id, const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;
  using wrapper_t = dish2::RootIDValWrapper<lcit_t>;

  return std::count(
    wrapper_t{ lcit_t::make_begin( population ) },
    wrapper_t{ lcit_t::make_end( population ) },
    root_id
  ) / static_cast<double>( std::distance(
    lcit_t::make_begin( population ),
    lcit_t::make_end( population )
  ) );


}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_ROOT_ID_PREVALENCE_HPP_INCLUDE
