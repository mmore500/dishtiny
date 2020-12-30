#pragma once
#ifndef DISH2_RECORD_DUMP_POPULATION_HPP_INCLUDE
#define DISH2_RECORD_DUMP_POPULATION_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/cereal/include/cereal/types/vector.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../world/iterators/GenotypeConstWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "make_filename/make_dump_population_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_population(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  const auto& population = world.population;

  const std::string filename(
    dish2::make_dump_population_filename( thread_idx )
  );

  std::ofstream os(filename, std::ios::binary);
  cereal::BinaryOutputArchive archive( os );

  archive( emp::vector< dish2::Genome<Spec> >(
    dish2::GenotypeConstWrapper<Spec>(
      dish2::LiveCellIterator<Spec>::make_begin( population )
    ),
    dish2::GenotypeConstWrapper<Spec>(
      dish2::LiveCellIterator<Spec>::make_end( population )
    )
  ) );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_POPULATION_HPP_INCLUDE
