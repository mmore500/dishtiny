#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_GENERATION_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_GENERATION_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/GenerationCounterConstWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

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

  return std::accumulate(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) },
    0.0,
    [lev]( const auto& accumulator, const auto& generation_counter ){
      return accumulator + generation_counter.elapsed_generations[ lev ];
    }
  ) / std::distance(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_GENERATION_HPP_INCLUDE
