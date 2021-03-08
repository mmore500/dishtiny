#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_PROGRAM_LENGTH_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_PROGRAM_LENGTH_HPP_INCLUDE

#include <algorithm>
#include <limits>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_mean_program_length( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  emp::vector< size_t > program_lengths;
  std::transform(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population ),
    std::back_inserter( program_lengths ),
    []( const dish2::Cell<Spec>& cell ){
      return cell.genome->program.size();
    }
  );

  if ( program_lengths.empty() ) {
    emp_assert( dish2::no_live_cells<Spec>( world ) );
    return std::numeric_limits<double>::quiet_NaN();
  } else return std::accumulate(
    std::begin( program_lengths ), std::end( program_lengths ),
    double{}
  ) / program_lengths.size();

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_PROGRAM_LENGTH_HPP_INCLUDE
