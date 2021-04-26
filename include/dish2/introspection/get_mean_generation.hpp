#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_GENERATION_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_GENERATION_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/GenerationCounterConstWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_live_cells.hpp"
#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_mean_generation(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  const auto& population = world.population;

  using wrapper_t = dish2::GenerationCounterConstWrapper<
    Spec,
    dish2::LiveCellIterator<Spec>
  >;

  if ( dish2::no_live_cells<Spec>( world ) ) {
    return std::numeric_limits<double>::quiet_NaN();
  } else return std::accumulate(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) },
    double{},
    [lev]( const auto& accumulator, const auto& generation_counter ){
      return accumulator + generation_counter.elapsed_generations[ lev ];
    }
  ) / dish2::count_live_cells< Spec >( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_GENERATION_HPP_INCLUDE
