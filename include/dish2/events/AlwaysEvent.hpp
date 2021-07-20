#pragma once
#ifndef DISH2_EVENTS_ALWAYSEVENT_HPP_INCLUDE
#define DISH2_EVENTS_ALWAYSEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * This event is always dispatched.
 */
struct AlwaysEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& ) { return true; }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_ALWAYSEVENT_HPP_INCLUDE
