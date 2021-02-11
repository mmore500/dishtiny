#pragma once
#ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../algorithm/make_phenotype_equivalent_nopout.hpp"
#include "../introspection/count_live_cells.hpp"
#include "../introspection/get_prevalent_coding_genotype_genome.hpp"
#include "../introspection/no_live_cells.hpp"

#include "../world/ThreadWorld.hpp"

#include "dump_genome.hpp"

#include "make_filename/make_dump_abundance_genome_filename.hpp"

namespace dish2 {

template< typename Spec >
bool dump_abundance_genome(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  // abort if no live cells
  if ( dish2::no_live_cells<Spec>(world) ) return false;

  const auto [genome, abundance_tally]
    = dish2::get_prevalent_coding_genotype_genome<Spec>( world );

  const double abundance_proportion{ abundance_tally / static_cast<double>(
    dish2::count_live_cells<Spec>( world )
  ) };

  dish2::dump_genome< Spec >(
    genome,
    dish2::make_dump_abundance_genome_filename(
      thread_idx, abundance_tally, abundance_proportion, "wildtype"
    )
  );

  if (
    cfg.PHENOTYPE_EQUIVALENT_NOPOUT() && thread_idx == 0 && uitsl::is_root()
  ) {
    std::cout << "recording phenotype equivalent nopout" << std::endl;
    dish2::dump_genome< Spec >(
      dish2::make_phenotype_equivalent_nopout< Spec >(
        genome, "abundance"
      ),
      dish2::make_dump_abundance_genome_filename(
        thread_idx, abundance_tally, abundance_proportion,
        "phenotype_equivalent_nopout"
      )
    );
  }

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
