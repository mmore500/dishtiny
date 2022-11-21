#pragma once
#ifndef DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../introspection/get_lowest_root_id.hpp"
#include "../introspection/no_live_cells.hpp"
#include "../world/iterators/LiveCellIterator.hpp"

#include "../world/ThreadWorld.hpp"

#include "dump_genome.hpp"

#include "make_filename/make_dump_arbitrary_genome_filename.hpp"

namespace dish2 {

template< typename Spec >
bool dump_arbitrary_genome( const dish2::ThreadWorld< Spec >& world ) {

  // abort if no live cells
  if ( dish2::no_live_cells<Spec>(world) ) return false;

  const size_t lowest_root_id = dish2::get_lowest_root_id<Spec>( world );

  // get the first /live/ cell with lowest root id
  const auto res = std::find_if(
    dish2::LiveCellIterator<Spec>::make_begin( world.population ),
    dish2::LiveCellIterator<Spec>::make_end( world.population ),
    [lowest_root_id]( const auto& cell ){
      return cell.genome->root_id.GetID() == lowest_root_id;
    }
  );

  const auto& genome = *(res->genome);

  dish2::dump_genome< Spec >(
    genome,
    dish2::make_dump_arbitrary_genome_filename( "wildtype" )
  );

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE
