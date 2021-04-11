#pragma once
#ifndef DISH2_INTROSPECTION_SUM_STATE_MESH_PROC_ATTEMPTED_PUTS_HPP_INCLUDE
#define DISH2_INTROSPECTION_SUM_STATE_MESH_PROC_ATTEMPTED_PUTS_HPP_INCLUDE

#include <numeric>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/cardinal_iterators/StateNodeOutputWrapper.hpp"
#include "../world/iterators/WorldIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t sum_state_mesh_proc_attempted_puts(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using iterator_t = dish2::WorldIteratorAdapter<
    typename emp::vector< dish2::Cell< Spec > >::const_iterator,
    dish2::StateNodeOutputWrapper<Spec>
  >;

  return std::accumulate(
    iterator_t::make_begin( std::begin(population) ),
    iterator_t::make_end( std::end(population) ),
    size_t{},
    [](const size_t accumulator, const auto& out){
      if ( !out.HoldsProcImpl().value() ) return accumulator;
      else return accumulator + out.GetAttemptedPutCount();
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_SUM_STATE_MESH_PROC_ATTEMPTED_PUTS_HPP_INCLUDE
