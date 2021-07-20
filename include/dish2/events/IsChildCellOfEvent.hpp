#pragma once
#ifndef DISH2_EVENTS_ISCHILDCELLOFEVENT_HPP_INCLUDE
#define DISH2_EVENTS_ISCHILDCELLOFEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Is this cell a daughter cell of the cardinal's neighbor?
 *
 * Triggered if this cell was spawned from the cardinal's neighbor and its cell
 * is younger than the neighbor.
 */
struct IsChildCellOfEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      dish2::IsChildCellOf
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_ISCHILDCELLOFEVENT_HPP_INCLUDE
