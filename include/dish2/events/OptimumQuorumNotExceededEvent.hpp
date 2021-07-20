#pragma once
#ifndef DISH2_EVENTS_OPTIMUMQUORUMNOTEXCEEDEDEVENT_HPP_INCLUDE
#define DISH2_EVENTS_OPTIMUMQUORUMNOTEXCEEDEDEVENT_HPP_INCLUDE

#include "OptimumQuorumExceededEvent.hpp"

namespace dish2 {

/**
 * Triggered if the cell's number of known quorum bits is less than or equal to
 * than the target quorum count.
 */
struct OptimumQuorumNotExceededEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return !dish2::OptimumQuorumExceededEvent::Test<Cardinal, SeriesIdx>( cardinal );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_OPTIMUMQUORUMNOTEXCEEDEDEVENT_HPP_INCLUDE
