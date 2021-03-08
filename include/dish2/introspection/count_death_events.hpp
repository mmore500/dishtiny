#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_DEATH_EVENTS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_DEATH_EVENTS_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../runninglog/DeathEvent.hpp"
#include "../world/iterators/LogIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t count_death_events( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  return std::accumulate(
    std::begin( population ), std::end( population ),
    size_t{},
    []( const size_t accumulator, const auto& cell ){
      const auto& log = cell.running_logs.template GetLog<
        dish2::DeathEvent<Spec>
      >();
      return accumulator + log.GetSize();
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_DEATH_EVENTS_HPP_INCLUDE
