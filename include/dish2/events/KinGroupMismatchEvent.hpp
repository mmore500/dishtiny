#pragma once
#ifndef DISH2_EVENTS_KINGROUPMISMATCHEVENT_HPP_INCLUDE
#define DISH2_EVENTS_KINGROUPMISMATCHEVENT_HPP_INCLUDE

#include "KinGroupMatchEvent.hpp"

namespace dish2 {

struct KinGroupMismatchEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return !KinGroupMatchEvent::Test<Cardinal, SeriesIdx>( cardinal );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_KINGROUPMISMATCHEVENT_HPP_INCLUDE
