#pragma once
#ifndef DISH2_INTROSPECTION_GET_POPULATION_COMPRESSION_RATIO_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_POPULATION_COMPRESSION_RATIO_HPP_INCLUDE

#include <algorithm>
#include <random>
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

  using lcit_t = dish2::LiveCellIterator<Spec>;
  using wrapper_t = dish2::GenotypeConstWrapper<Spec>;
  const auto begin = wrapper_t{ lcit_t::make_begin( world.population ) };
  const auto end = wrapper_t{ lcit_t::make_end( world.population ) };

  emp::vector< dish2::Genome<Spec> > sample;

  std::sample(
    begin, end,
    std::back_inserter( sample ),
    20,
    std::mt19937{}
  );

  return dish2::measure_compression_ratio([&](){
    std::ostringstream ss;
    {
      cereal::BinaryOutputArchive archive( ss );
      archive( sample );
    }
    return ss.str();
  }());

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_POPULATION_COMPRESSION_RATIO_HPP_INCLUDE
