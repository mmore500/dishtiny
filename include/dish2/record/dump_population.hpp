#pragma once
#ifndef DISH2_RECORD_DUMP_POPULATION_HPP_INCLUDE
#define DISH2_RECORD_DUMP_POPULATION_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/cereal/include/cereal/types/vector.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../debug/log_msg.hpp"
#include "../genome/Genome.hpp"
#include "../utility/pare_keyname_filename.hpp"
#include "../world/iterators/GenotypeConstWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "make_filename/make_artifact_path.hpp"
#include "make_filename/make_dump_population_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_population( const dish2::ThreadWorld< Spec >& world ) {

  const auto& population = world.population;

  const std::string out_filename( dish2::pare_keyname_filename(
    dish2::make_dump_population_filename(),
    dish2::make_artifact_path()
  ) );

  bxz::ofstream os( dish2::make_artifact_path( out_filename ), bxz::lzma, 9);
  cereal::BinaryOutputArchive archive( os );

  archive( emp::vector< dish2::Genome<Spec> >(
    dish2::GenotypeConstWrapper<Spec>(
      dish2::LiveCellIterator<Spec>::make_begin( population )
    ),
    dish2::GenotypeConstWrapper<Spec>(
      dish2::LiveCellIterator<Spec>::make_end( population )
    )
  ) );

  dish2::log_msg( "dumped population" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_POPULATION_HPP_INCLUDE
