#pragma once
#ifndef DISH2_RECORD_DUMP_ABUNDANCE_LOWEST_ROOT_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ABUNDANCE_LOWEST_ROOT_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../algorithm/make_battleship_phenotype_equivalent_nopout.hpp"
#include "../algorithm/make_jenga_phenotype_equivalent_nopout.hpp"
#include "../debug/log_msg.hpp"
#include "../genome/Genome.hpp"
#include "../introspection/count_live_cells.hpp"
#include "../introspection/get_lowest_root_prevalent_coding_genotype_genome.hpp"
#include "../introspection/no_live_cells.hpp"
#include "../world/iterators/GenotypeConstWrapper.hpp"
#include "../world/iterators/LiveCellExceptFocalRootIDIterator.hpp"
#include "../world/ThreadWorld.hpp"

#include "dump_genome.hpp"

#include "make_filename/make_dump_abundance_genome_filename.hpp"

namespace dish2 {

// dumps most abundant genome from the lowest phylogenetic root
template< typename Spec >
bool dump_abundance_lowest_root_genome(const dish2::ThreadWorld<Spec>& world) {

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

  const auto& population = world.population;
  using lcefrit_t = dish2::LiveCellExceptFocalRootIDIterator<Spec>;
  using wrapper_t = dish2::GenotypeConstWrapper<
    Spec,
    lcefrit_t
  >;
  const size_t focal_root_id = genome.root_id;

  const emp::vector<dish2::Genome<Spec>> background_population{
    wrapper_t{ lcefrit_t::make_begin(population, focal_root_id) },
    wrapper_t{ lcefrit_t::make_end(population, focal_root_id) }
  };
  dish2::log_msg( "background population size", background_population.size() );

  if (
    cfg.BATTLESHIP_PHENOTYPE_EQUIVALENT_NOPOUT_LOWEST_ROOT()
    && dish2::thread_idx == 0
    && uitsl::is_root()
  ) {
    dish2::log_msg( "recording battleship phenotype equivalent nopout" );
    dish2::dump_genome< Spec >(
      dish2::make_battleship_phenotype_equivalent_nopout< Spec >(
        genome, "abundance", background_population
      ),
      dish2::make_dump_abundance_genome_filename(
        count, abundance, prevalence,
        background_population.size()
          ? "battleship_phenotype_equivalent_nopout_with_biotic_background"
          : "battleship_phenotype_equivalent_nopout",
          background_population.size()
      )
    );
  }

  if (
    (
      cfg.PHENOTYPE_EQUIVALENT_NOPOUT_LOWEST_ROOT()
      || cfg.JENGA_PHENOTYPE_EQUIVALENT_NOPOUT_LOWEST_ROOT()
    )
    && dish2::thread_idx == 0
    && uitsl::is_root()
  ) {
    dish2::log_msg( "recording jenga phenotype equivalent nopout" );

    const auto res = dish2::make_jenga_phenotype_equivalent_nopout< Spec >(
      genome, "abundance", background_population
    );

    dish2::dump_genome< Spec >(
      res,
      dish2::make_dump_abundance_genome_filename(
        count, abundance, prevalence,
        "jenga_phenotype_equivalent_nopout",
        background_population.size()
      )
    );
    dish2::dump_genome< Spec >(
      res,
      dish2::make_dump_abundance_genome_filename(
        count, abundance, prevalence,
        "phenotype_equivalent_nopout",
        background_population.size()
      )
    );
  }

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ABUNDANCE_LOWEST_ROOT_GENOME_HPP_INCLUDE
