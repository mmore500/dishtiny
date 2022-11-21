#pragma once
#ifndef DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE

#include "dump_arbitrary_lowest_root_genome.hpp"

namespace dish2 {

template< typename Spec >
[[deprecated]]
bool dump_arbitrary_genome( const dish2::ThreadWorld< Spec >& world ) {
  return dish2::dump_arbitrary_lowest_root_genome(world);
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE
