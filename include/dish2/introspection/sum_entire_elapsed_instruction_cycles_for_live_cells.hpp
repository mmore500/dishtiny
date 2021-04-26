#pragma once
#ifndef DISH2_INTROSPECTION_SUM_ENTIRE_ELAPSED_INSTRUCTION_CYCLES_FOR_LIVE_CELLS_HPP_INCLUDE
#define DISH2_INTROSPECTION_SUM_ENTIRE_ELAPSED_INSTRUCTION_CYCLES_FOR_LIVE_CELLS_HPP_INCLUDE

#include <algorithm>
#include <limits>

#include "../cell/cardinal_iterators/EntireElapsedInstructionCyclesWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/WorldIteratorValueAdapter.hpp"
#include "../world/ThreadWorld.hpp"

#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
size_t sum_entire_elapsed_instruction_cycles_for_live_cells(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  using iterator_t = dish2::WorldIteratorValueAdapter<
    lcit_t,
    dish2::EntireElapsedInstructionCyclesWrapper<Spec>
  >;

  if ( dish2::no_live_cells<Spec>( world ) ) return 0;
  else return std::accumulate(
    iterator_t::make_begin( lcit_t::make_begin( population ) ),
    iterator_t::make_end( lcit_t::make_end( population ) ),
    size_t{}
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_SUM_ENTIRE_ELAPSED_INSTRUCTION_CYCLES_FOR_LIVE_CELLS_HPP_INCLUDE
