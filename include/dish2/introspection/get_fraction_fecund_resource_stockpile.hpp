#pragma once
#ifndef DISH2_INTROSPECTION_GET_FRACTION_FECUND_RESOURCE_STOCKPILE_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_FRACTION_FECUND_RESOURCE_STOCKPILE_HPP_INCLUDE

#include <algorithm>

#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/WorldIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
double get_fraction_fecund_resource_stockpile(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  using iterator_t = dish2::WorldIteratorAdapter<
    lcit_t,
    dish2::ResourceStockpileWrapper<Spec>
  >;

  return std::count_if(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) ),
    []( const double stockpile ){  return stockpile >= 1.0; }
  ) / static_cast< double >( std::distance(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) )
  ) );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_FRACTION_FECUND_RESOURCE_STOCKPILE_HPP_INCLUDE
