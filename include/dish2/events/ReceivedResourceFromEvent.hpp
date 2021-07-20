#pragma once
#ifndef DISH2_EVENTS_RECEIVEDRESOURCEFROMEVENT_HPP_INCLUDE
#define DISH2_EVENTS_RECEIVEDRESOURCEFROMEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Triggered if a cardinal's cell has received resource from that cardinal's
 * neighbor cell.
 */
struct ReceivedResourceFromEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      typename dish2::ReceivedResourceFrom
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_RECEIVEDRESOURCEFROMEVENT_HPP_INCLUDE
