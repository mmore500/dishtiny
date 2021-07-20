#pragma once
#ifndef DISH2_EVENTS_STOCKPILEFECUNDEVENT_HPP_INCLUDE
#define DISH2_EVENTS_STOCKPILEFECUNDEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Does this cell have enough stockpiled resource to fund cellular reproduction?
 *
 * Triggered if a cell's stockpile is greater than 1.0.
 */
struct StockpileFecundEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      dish2::StockpileFecund
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_STOCKPILEFECUNDEVENT_HPP_INCLUDE
