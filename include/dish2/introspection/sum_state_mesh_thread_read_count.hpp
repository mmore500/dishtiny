#pragma once
#ifndef DISH2_INTROSPECTION_SUM_STATE_MESH_THREAD_READ_COUNT_HPP_INCLUDE
#define DISH2_INTROSPECTION_SUM_STATE_MESH_THREAD_READ_COUNT_HPP_INCLUDE

#include <numeric>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/cardinal_iterators/StateNodeInputWrapper.hpp"
#include "../world/iterators/WorldIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t sum_state_mesh_thread_read_count(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using iterator_t = dish2::WorldIteratorAdapter<
    typename emp::vector< dish2::Cell< Spec > >::const_iterator,
    dish2::StateNodeInputWrapper<Spec>
  >;

  return std::accumulate(
    iterator_t::make_begin( std::begin(population) ),
    iterator_t::make_end( std::end(population) ),
    size_t{},
    [](const size_t accumulator, const auto& in){
      if ( !in.HoldsThreadImpl().value() ) return accumulator;
      else return accumulator + in.GetReadCount();
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_SUM_STATE_MESH_THREAD_READ_COUNT_HPP_INCLUDE
