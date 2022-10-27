#pragma once
#ifndef DISH2_EVENTS_PHYLOGENETICROOTMISMATCHEVENT_HPP_INCLUDE
#define DISH2_EVENTS_PHYLOGENETICROOTMISMATCHEVENT_HPP_INCLUDE

#include "PhylogeneticRootMatchEvent.hpp"

namespace dish2 {

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
