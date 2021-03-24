#pragma once
#ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../algorithm/make_phenotype_equivalent_nopout.hpp"
#include "../introspection/count_live_cells.hpp"
#include "../introspection/get_lowest_root_prevalent_coding_genotype_genome.hpp"
#include "../introspection/no_live_cells.hpp"

#include "../config/thread_idx.hpp"
#include "../world/ThreadWorld.hpp"

#include "dump_genome.hpp"

#include "make_filename/make_dump_abundance_genome_filename.hpp"

namespace dish2 {

// dumps most abundant genome from the lowest phylogenetic root
template< typename Spec >
bool dump_abundance_genome( const dish2::ThreadWorld< Spec >& world ) {

  // abort if no live cells
  if ( dish2::no_live_cells<Spec>(world) ) return false;

  const auto [genome, count]
    = dish2::get_lowest_root_prevalent_coding_genotype_genome<Spec>( world );

  const double abundance{ count / static_cast<double>(
    world.GetSize()
  ) };

  const double prevalence{ count / static_cast<double>(
    dish2::count_live_cells<Spec>( world )
  ) };

  dish2::dump_genome< Spec >(
    genome,
    dish2::make_dump_abundance_genome_filename(
      count, abundance, prevalence, "wildtype"
    )
  );

  if (
    cfg.PHENOTYPE_EQUIVALENT_NOPOUT()
    && dish2::thread_idx == 0
    && uitsl::is_root()
  ) {
    std::cout << "recording phenotype equivalent nopout" << '\n';
    dish2::dump_genome< Spec >(
      dish2::make_phenotype_equivalent_nopout< Spec >(
        genome, "abundance"
      ),
      dish2::make_dump_abundance_genome_filename(
        count, abundance, prevalence,
        "phenotype_equivalent_nopout"
      )
    );
  }

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
