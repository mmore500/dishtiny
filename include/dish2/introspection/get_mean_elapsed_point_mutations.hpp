#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_ELAPSED_POINT_MUTATIONS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_ELAPSED_POINT_MUTATIONS_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/ElapsedPointMutationsConstWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
double get_mean_elapsed_point_mutations(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using wrapper_t = dish2::ElapsedPointMutationsConstWrapper<
    Spec,
    dish2::LiveCellIterator<Spec>
  >;

  return std::accumulate(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) },
    0.0,
    []( const auto& accumulator, const auto& counter ){
      return accumulator + counter;
    }
  ) / std::distance(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_ELAPSED_POINT_MUTATIONS_HPP_INCLUDE
