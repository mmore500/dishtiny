#pragma once
#ifndef DISH2_RECORD_DUMP_ARBITRARY_OVER_ALL_ROOTS_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ARBITRARY_OVER_ALL_ROOTS_GENOME_HPP_INCLUDE

#include <string>

#include "../introspection/no_live_cells.hpp"
#include "../world/iterators/LiveCellIterator.hpp"

#include "../world/ThreadWorld.hpp"

#include "dump_genome.hpp"

#include "make_filename/make_dump_arbitrary_over_all_roots_genome_filename.hpp"

namespace dish2 {

template< typename Spec >
bool dump_arbitrary_over_all_roots_genome(
  const dish2::ThreadWorld< Spec >& world
) {

  // abort if no live cells
  if ( dish2::no_live_cells<Spec>(world) ) return false;

  // get the first /live/ cell
  const auto& genome
    = *(dish2::LiveCellIterator<Spec>::make_begin( world.population )->genome);

  dish2::dump_genome< Spec >(
    genome,
    dish2::make_dump_arbitrary_over_all_roots_genome_filename( "wildtype" )
  );

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ARBITRARY_OVER_ALL_ROOTS_GENOME_HPP_INCLUDE
