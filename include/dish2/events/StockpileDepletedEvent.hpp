#pragma once
#ifndef DISH2_EVENTS_STOCKPILEDEPLETEDEVENT_HPP_INCLUDE
#define DISH2_EVENTS_STOCKPILEDEPLETEDEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Is this cell's stockpile empty?
 *
 * Triggered if a cell's stockpile is less than twice the base harvest rate.
 */
struct StockpileDepletedEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      dish2::StockpileDepleted
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_STOCKPILEDEPLETEDEVENT_HPP_INCLUDE
