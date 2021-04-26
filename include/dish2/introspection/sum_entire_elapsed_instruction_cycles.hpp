#pragma once
#ifndef DISH2_INTROSPECTION_SUM_ENTIRE_ELAPSED_INSTRUCTION_CYCLES_HPP_INCLUDE
#define DISH2_INTROSPECTION_SUM_ENTIRE_ELAPSED_INSTRUCTION_CYCLES_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/cardinal_iterators/EntireElapsedInstructionCyclesWrapper.hpp"
#include "../cell/Cell.hpp"
#include "../world/iterators/WorldIteratorValueAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t sum_entire_elapsed_instruction_cycles(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using iterator_t = dish2::WorldIteratorValueAdapter<
    typename emp::vector< dish2::Cell< Spec > >::const_iterator,
    dish2::EntireElapsedInstructionCyclesWrapper<Spec>
  >;

  return std::accumulate(
    iterator_t::make_begin( std::begin(population) ),
    iterator_t::make_end( std::end(population) ),
    size_t{}
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_SUM_ENTIRE_ELAPSED_INSTRUCTION_CYCLES_HPP_INCLUDE
