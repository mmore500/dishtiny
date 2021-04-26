#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_ELAPSED_MUTATION_OCCURENCES_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_ELAPSED_MUTATION_OCCURENCES_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/ElapsedMutationOccurencesConstWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_live_cells.hpp"
#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_mean_elapsed_mutation_occurences(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using wrapper_t = dish2::ElapsedMutationOccurencesConstWrapper<
    Spec,
    dish2::LiveCellIterator<Spec>
  >;

  if ( dish2::no_live_cells<Spec>( world ) ) {
    return std::numeric_limits<double>::quiet_NaN();
  } else return std::accumulate(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) },
    double{},
    []( const auto& accumulator, const auto& counter ){
      return accumulator + counter;
    }
  ) / dish2::count_live_cells< Spec >( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_ELAPSED_MUTATION_OCCURENCES_HPP_INCLUDE
