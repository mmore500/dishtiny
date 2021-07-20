#pragma once
#ifndef DISH2_EVENTS_POORERTHANNEIGHBOREVENT_HPP_INCLUDE
#define DISH2_EVENTS_POORERTHANNEIGHBOREVENT_HPP_INCLUDE

#include "RicherThanNeighborEvent.hpp"

namespace dish2 {

/**
 * Does this cell have less resource stockpiled than its neighbor?
 *
 * Triggered if a cardinal's cell has less resource than that cardinal's
 * neighbor cell.
 */
struct PoorerThanNeighborEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return !dish2::RicherThanNeighborEvent::Test<Cardinal, SeriesIdx>(
      cardinal
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_POORERTHANNEIGHBOREVENT_HPP_INCLUDE
