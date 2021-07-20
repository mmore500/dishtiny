#pragma once
#ifndef DISH2_EVENTS_NEIGHBORAPOPTOSISEVENT_HPP_INCLUDE
#define DISH2_EVENTS_NEIGHBORAPOPTOSISEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Triggered if the most recent cell death in the cardinal's neighbor tile was
 * apoptosis.
 */
struct NeighborApoptosisEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      dish2::NeighborApoptosis
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_NEIGHBORAPOPTOSISEVENT_HPP_INCLUDE
