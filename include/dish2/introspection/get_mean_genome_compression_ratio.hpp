#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_GENOME_COMPRESSION_RATIO_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_GENOME_COMPRESSION_RATIO_HPP_INCLUDE

#include <algorithm>
#include <limits>
#include <random>
#include <sstream>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../utility/measure_compression_ratio.hpp"
#include "../world/iterators/GenotypeConstWrapper.hpp"
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

  emp::vector< dish2::Genome<Spec> > sample;

  using lcit_t = dish2::LiveCellIterator<Spec>;
  const auto begin = dish2::GenotypeConstWrapper<Spec>(
    lcit_t::make_begin( population )
  );
  const auto end = dish2::GenotypeConstWrapper<Spec>(
    lcit_t::make_end( population )
  );

  std::sample(
    begin, end,
    std::back_inserter( sample ),
    20,
    std::mt19937{}
  );


  if ( dish2::no_live_cells<Spec>( world ) ) {
    return std::numeric_limits<double>::quiet_NaN();
  } else return std::accumulate(
    std::begin( sample ), std::end( sample ),
    double{},
    [](const double accum, const auto& genome){
      return accum + dish2::measure_compression_ratio([&](){
        std::ostringstream ss;
        {
          cereal::BinaryOutputArchive archive( ss );
          archive( genome );
        }
        return ss.str();
      }());
    }
  ) / sample.size();

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_GENOME_COMPRESSION_RATIO_HPP_INCLUDE
