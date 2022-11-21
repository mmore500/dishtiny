#pragma once
#ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE

#include "dump_abundance_lowest_root_genome.hpp"

namespace dish2 {

// dumps most abundant genome from the lowest phylogenetic root
template< typename Spec >
[[deprecated]]
bool dump_abundance_genome( const dish2::ThreadWorld< Spec >& world ) {
  return dish2::dump_abundance_lowest_root_genome<Spec>(world);
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ABUNDANCE_GENOME_HPP_INCLUDE
