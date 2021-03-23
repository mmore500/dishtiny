#pragma once
#ifndef DISH2_EVENTS_NEIGHBORISNOTALIVEEVENT_HPP_INCLUDE
#define DISH2_EVENTS_NEIGHBORISNOTALIVEEVENT_HPP_INCLUDE

#include "NeighborIsAliveEvent.hpp"

namespace dish2 {

struct NeighborIsNotAliveEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return !dish2::NeighborIsAliveEvent::Test<Cardinal, SeriesIdx>( cardinal );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_NEIGHBORISNOTALIVEEVENT_HPP_INCLUDE
