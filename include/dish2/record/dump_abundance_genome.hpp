#pragma once
#ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/json.hpp"

#include "../introspection/count_live_cells.hpp"
#include "../introspection/get_prevalent_coding_genotype_genome.hpp"

#include "../world/ThreadWorld.hpp"

#include "make_dump_abundance_genome_filename.hpp"

namespace dish2 {

template< typename Spec >
bool dump_abundance_genome(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  // abort if no live cells
  if ( dish2::count_live_cells<Spec>(world) == 0 ) return false;

  const auto [genome, abundance]
    = dish2::get_prevalent_coding_genotype_genome<Spec>( world );

  const double abundance_frac{
    abundance / static_cast<double>( dish2::count_live_cells<Spec>( world ) )
  };

  const std::string filename(
    dish2::make_dump_abundance_genome_filename( thread_idx, abundance_frac )
  );

  std::ofstream os( filename );
  cereal::JSONOutputArchive archive( os );

  archive( genome );

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
