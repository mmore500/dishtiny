#pragma once
#ifndef DISH2_RECORD_DUMP_IN_PROGRESS_JENGA_NOPOUT_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_IN_PROGRESS_JENGA_NOPOUT_GENOME_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../genome/Genome.hpp"

#include "dump_genome.hpp"

#include "make_filename/make_in_progress_jenga_nopout_genome_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_in_progress_jenga_nopout_genome( const dish2::Genome<Spec>& genome ) {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  dish2::dump_genome< Spec >(
    genome,
    dish2::make_in_progress_jenga_nopout_genome_filename(
      sgpl::count_op_instructions<sgpl_spec_t>( genome.program )
    )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_IN_PROGRESS_JENGA_NOPOUT_GENOME_HPP_INCLUDE
