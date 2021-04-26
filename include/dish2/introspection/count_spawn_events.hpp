#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_SPAWN_EVENTS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_SPAWN_EVENTS_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../runninglog/SpawnEvent.hpp"
#include "../world/iterators/LogIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t count_spawn_events( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  return std::accumulate(
    std::begin( population ), std::end( population ),
    size_t{},
    []( const size_t accumulator, const auto& cell ){
      const auto& log = cell.running_logs.template GetLog<
        dish2::SpawnEvent<Spec>
      >();
      return accumulator + log.GetSize();
    }
  );

}

template< typename Spec >
size_t count_spawn_events(
  const dish2::ThreadWorld<Spec>& world, const size_t replev
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
      return accumulator + ( event.replev == replev );
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_SPAWN_EVENTS_HPP_INCLUDE
