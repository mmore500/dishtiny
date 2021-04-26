#pragma once
#ifndef DISH2_INTROSPECTION_GET_FRACTION_CARDINALS_RECEIVING_RESOURCE_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_FRACTION_CARDINALS_RECEIVING_RESOURCE_HPP_INCLUDE

#include <algorithm>
#include <limits>

#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/ResourceInputPeekWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/WorldIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_live_cardinals.hpp"
#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_fraction_cardinals_receiving_resource(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  using iterator_t = dish2::WorldIteratorAdapter<
    lcit_t,
    dish2::ResourceInputPeekWrapper<Spec>
  >;

  if ( dish2::no_live_cells<Spec>( world ) ) {
    return std::numeric_limits<double>::quiet_NaN();
  } else return std::count_if(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) ),
    std::identity
  ) / static_cast< double >( dish2::count_live_cardinals<Spec>( world ) );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_FRACTION_CARDINALS_RECEIVING_RESOURCE_HPP_INCLUDE
