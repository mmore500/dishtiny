#pragma once
#ifndef DISH2_EVENTS_ISPARENTCELLOFEVENT_HPP_INCLUDE
#define DISH2_EVENTS_ISPARENTCELLOFEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Is this cardinal's cell the parent cell of the cardinal's neighbor?
 *
 * Triggered if neighbor was spawned from cell and cell age is greater than
 * neighbor age.
 */
struct IsParentCellOfEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      dish2::IsParentCellOf
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_ISPARENTCELLOFEVENT_HPP_INCLUDE
