#pragma once
#ifndef DISH2_EVENTS_RICHERTHANNEIGHBOREVENT_HPP_INCLUDE
#define DISH2_EVENTS_RICHERTHANNEIGHBOREVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Does this cell have more resource stockpiled than its neighbor?
 *
 * Triggered if a cardinal's cell has more resource in its stockpile than that
 * cardinal's neighbor cell.
 */
struct RicherThanNeighborEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      typename dish2::RicherThanNeighbor
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_RICHERTHANNEIGHBOREVENT_HPP_INCLUDE
