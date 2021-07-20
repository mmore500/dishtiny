#pragma once
#ifndef DISH2_EVENTS_ISNEWBORNEVENT_HPP_INCLUDE
#define DISH2_EVENTS_ISNEWBORNEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * This event is dispatched once when a cell is first born.
 *
 * Triggered if cell age is less than frequency at which events are launched.
 */
struct IsNewbornEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      dish2::IsNewborn
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_ISNEWBORNEVENT_HPP_INCLUDE
