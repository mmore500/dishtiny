#pragma once
#ifndef DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../introspection/no_live_cells.hpp"
#include "../world/iterators/LiveCellIterator.hpp"

#include "../world/ThreadWorld.hpp"

#include "dump_genome.hpp"

#include "make_filename/make_dump_arbitrary_genome_filename.hpp"

namespace dish2 {

template< typename Spec >
bool dump_arbitrary_genome(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  // abort if no live cells
  if ( dish2::no_live_cells<Spec>(world) ) return false;

  // get the first /live/ cell
  const auto genome
    = *(dish2::LiveCellIterator<Spec>::make_begin( world.population )->genome);

  dish2::dump_genome< Spec >(
    genome,
    dish2::make_dump_arbitrary_genome_filename( thread_idx, "wildtype" )
  );

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE
