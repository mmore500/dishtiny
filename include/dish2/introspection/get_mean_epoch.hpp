#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_EPOCH_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_EPOCH_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/cardinal_iterators/EpochWrapper.hpp"
#include "../cell/Cell.hpp"
#include "../world/iterators/WorldIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
double get_mean_epoch( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  using iterator_t = dish2::WorldIteratorAdapter<
    typename std::vector< dish2::Cell< Spec > >::const_iterator,
    dish2::EpochWrapper<Spec>
  >;

  return std::accumulate(
    iterator_t::make_begin( std::begin(population) ),
    iterator_t::make_end( std::end(population) ),
    0.0
  ) / std::distance(
    iterator_t::make_begin( std::begin(population) ),
    iterator_t::make_end( std::end(population) )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_EPOCH_HPP_INCLUDE
