#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_GENOME_COMPRESSION_RATIO_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_GENOME_COMPRESSION_RATIO_HPP_INCLUDE

#include <algorithm>
#include <limits>
#include <sstream>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"

#include "../utility/measure_compression_ratio.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "count_live_cells.hpp"
#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
double get_mean_genome_compression_ratio(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  if ( dish2::no_live_cells<Spec>( world ) ) {
    return std::numeric_limits<double>::quiet_NaN();
  } else return std::accumulate(
    lcit_t::make_begin( population ),
    lcit_t::make_end( population ),
    0.0,
    [](const double accum, const auto& cell){
      return accum + dish2::measure_compression_ratio([&](){
        std::ostringstream ss;
        cereal::BinaryOutputArchive archive( ss );
        archive( *cell.genome );
        return ss.str();
      }());
    }
  ) / dish2::count_live_cells<Spec>( world );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_GENOME_COMPRESSION_RATIO_HPP_INCLUDE
