#pragma once
#ifndef DISH2_EVENTS_PHYLOGENETICROOTMATCHEVENT_HPP_INCLUDE
#define DISH2_EVENTS_PHYLOGENETICROOTMATCHEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Does this cell descend from the same originally-generated genome as its neighbor?
 *
 * Triggered if a cardinal's cell's root ID is equal to that cardinal's
 * neighbor cell's root ID.
 */
struct PhylogeneticRootMatchEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      typename dish2::PhylogeneticRootMatch
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_PHYLOGENETICROOTMATCHEVENT_HPP_INCLUDE
