#pragma once
#ifndef DISH2_EVENTS_PHYLOGENETICROOTMATCHEVENT_HPP_INCLUDE
#define DISH2_EVENTS_PHYLOGENETICROOTMATCHEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

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
