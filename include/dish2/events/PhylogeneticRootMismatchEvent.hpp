#pragma once
#ifndef DISH2_EVENTS_PHYLOGENETICROOTMISMATCHEVENT_HPP_INCLUDE
#define DISH2_EVENTS_PHYLOGENETICROOTMISMATCHEVENT_HPP_INCLUDE

#include "PhylogeneticRootMatchEvent.hpp"

namespace dish2 {

/**
 * Does this cell and its neighbor descend from a different originally-generated genomes?
 *
 * Triggered if a cardinal's cell's root ID is not equal to that cardinal's
 * neighbor cell's Root ID.
 */
struct PhylogeneticRootMismatchEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return !dish2::PhylogeneticRootMatchEvent::Test<Cardinal, SeriesIdx>(
      cardinal
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_PHYLOGENETICROOTMISMATCHEVENT_HPP_INCLUDE
