#pragma once
#ifndef DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_KIN_ELIMINATED_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_KIN_ELIMINATED_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../runninglog/SpawnEvent.hpp"
#include "../world/iterators/LogIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t get_total_spawn_event_kin_eliminated(
  const dish2::ThreadWorld<Spec>& world,
  const size_t kin_id_commonality_parent_eliminated
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
      return accumulator + (
        event.kin_id_commonality_parent_eliminated
        >= kin_id_commonality_parent_eliminated
      );
    }
  );

}

template< typename Spec >
size_t get_total_spawn_event_kin_eliminated(
  const dish2::ThreadWorld<Spec>& world,
  const size_t kin_id_commonality_parent_eliminated,
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
      return accumulator + (
        event.replev == replev
        && event.kin_id_commonality_parent_eliminated
          >= kin_id_commonality_parent_eliminated
      );
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_KIN_ELIMINATED_HPP_INCLUDE
