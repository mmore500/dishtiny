#pragma once
#ifndef DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_NEIGHBORS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_NEIGHBORS_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../runninglog/SpawnEvent.hpp"
#include "../world/iterators/LogIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t get_total_spawn_event_neighbors(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using iterator_t = dish2::LogIteratorAdapter<
    dish2::SpawnEvent<Spec>,
    decltype( std::begin(population) )
  >;

  const auto [begin, end] = iterator_t::make(
    std::begin( population ), std::end( population )
  );

  return std::accumulate(
    begin, end, size_t{},
    []( const size_t accumulator, const auto& event ){
      return accumulator + event.num_neighbors_parent;
    }
  );

}

template< typename Spec >
size_t get_total_spawn_event_neighbors(
  const dish2::ThreadWorld<Spec>& world,
  const size_t replev
) {

  const auto& population = world.population;

  using iterator_t = dish2::LogIteratorAdapter<
    dish2::SpawnEvent<Spec>,
    decltype( std::begin(population) )
  >;

  const auto [begin, end] = iterator_t::make(
    std::begin( population ), std::end( population )
  );

  return std::accumulate(
    begin, end, size_t{},
    [=]( const size_t accumulator, const auto& event ){
      return accumulator
        + event.num_neighbors_parent * ( event.replev == replev );
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_NEIGHBORS_HPP_INCLUDE
