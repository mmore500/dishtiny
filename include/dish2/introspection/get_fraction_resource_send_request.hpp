#pragma once
#ifndef DISH2_INTROSPECTION_GET_FRACTION_RESOURCE_SEND_REQUEST_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_FRACTION_RESOURCE_SEND_REQUEST_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/ResourceSendRequestWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/WorldIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
double get_fraction_resource_send_request(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  using iterator_t = dish2::WorldIteratorAdapter<
    lcit_t,
    dish2::ResourceSendRequestWrapper<Spec>
  >;

  return std::count_if(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) ),
    std::identity
  ) / static_cast< double >( std::distance(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) )
  ) );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_FRACTION_RESOURCE_SEND_REQUEST_HPP_INCLUDE
