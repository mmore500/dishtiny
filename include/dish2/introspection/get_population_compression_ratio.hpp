#pragma once
#ifndef DISH2_INTROSPECTION_GET_POPULATION_COMPRESSION_RATIO_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_POPULATION_COMPRESSION_RATIO_HPP_INCLUDE

#include <sstream>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"

#include "../genome/Genome.hpp"
#include "../utility/measure_compression_ratio.hpp"
#include "../world/iterators/GenotypeConstWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
double get_population_compression_ratio(
  const dish2::ThreadWorld<Spec>& world
) {

  return dish2::measure_compression_ratio([&](){
    std::ostringstream ss;
    cereal::BinaryOutputArchive archive( ss );
    archive( emp::vector< dish2::Genome<Spec> >(
      dish2::GenotypeConstWrapper<Spec>(
        dish2::LiveCellIterator<Spec>::make_begin( world.population )
      ),
      dish2::GenotypeConstWrapper<Spec>(
        dish2::LiveCellIterator<Spec>::make_end( world.population )
      )
    ) );
    return ss.str();
  }());

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_POPULATION_COMPRESSION_RATIO_HPP_INCLUDE
