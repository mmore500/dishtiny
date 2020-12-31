#pragma once
#ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../algorithm/make_phenotype_equivalent_nopout.hpp"
#include "../introspection/count_live_cells.hpp"
#include "../introspection/get_prevalent_coding_genotype_genome.hpp"

#include "../world/ThreadWorld.hpp"

#include "make_filename/make_dump_abundance_genome_filename.hpp"

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

  {
    const std::string filename = dish2::make_dump_abundance_genome_filename(
      thread_idx, abundance_frac, "wildtype"
    );
    std::ofstream os( filename );
    cereal::JSONOutputArchive archive( os );
    archive( genome );
  }

  if (
    cfg.PHENOTYPE_EQUIVALENT_NOPOUT() && thread_idx == 0 && uitsl::is_root()
  ) {
    std::cout << "recording phenotype equivalent nopout" << std::endl;
    const std::string filename = dish2::make_dump_abundance_genome_filename(
      thread_idx, abundance_frac, "phenotype_equivalent_nopout"
    );
    std::ofstream os( filename );
    cereal::JSONOutputArchive archive( os );
    archive( dish2::make_phenotype_equivalent_nopout< Spec >(genome) );
  }

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
