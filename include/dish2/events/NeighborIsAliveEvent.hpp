#pragma once
#ifndef DISH2_EVENTS_NEIGHBORISALIVEEVENT_HPP_INCLUDE
#define DISH2_EVENTS_NEIGHBORISALIVEEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Triggered if a cardinal's neighbor tile is occupied by a live cell.
 */
struct NeighborIsAliveEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return std::as_const(
      cardinal.peripheral.state_node_input
    ).Get().template Get<
      dish2::IsAlive
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_NEIGHBORISALIVEEVENT_HPP_INCLUDE
