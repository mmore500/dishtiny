#pragma once
#ifndef DISH2_EVENTS_NEIGHBORISNEWBORNEVENT_HPP_INCLUDE
#define DISH2_EVENTS_NEIGHBORISNEWBORNEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

struct NeighborIsNewbornEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return cardinal.peripheral.readable_state.template Get<
      typename dish2::NeighborIsNewborn
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_NEIGHBORISNEWBORNEVENT_HPP_INCLUDE
