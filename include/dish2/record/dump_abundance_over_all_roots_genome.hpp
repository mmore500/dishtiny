#pragma once
#ifndef DISH2_RECORD_DUMP_ABUNDANCE_OVER_ALL_ROOTS_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ABUNDANCE_OVER_ALL_ROOTS_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../introspection/count_live_cells.hpp"
#include "../introspection/get_prevalent_coding_genotype_genome.hpp"
#include "../introspection/no_live_cells.hpp"

#include "../config/thread_idx.hpp"
#include "../world/ThreadWorld.hpp"

#include "dump_genome.hpp"

#include "make_filename/make_dump_abundance_over_all_roots_genome_filename.hpp"

namespace dish2 {

// dumps most abundant genome across all phylogenetic roots
template< typename Spec >
bool dump_abundance_over_all_roots_genome( const dish2::ThreadWorld< Spec >& world ) {

  // abort if no live cells
  if ( dish2::no_live_cells<Spec>(world) ) return false;

  const auto [genome, count]
    = dish2::get_prevalent_coding_genotype_genome<Spec>( world );

  const double abundance{ count / static_cast<double>(
    world.GetSize()
  ) };

  const double prevalence{ count / static_cast<double>(
    dish2::count_live_cells<Spec>( world )
  ) };

  dish2::dump_genome< Spec >(
    genome,
    dish2::make_dump_abundance_over_all_roots_genome_filename(
      count, abundance, prevalence, "wildtype"
    )
  );

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ABUNDANCE_OVER_ALL_ROOTS_GENOME_HPP_INCLUDE
