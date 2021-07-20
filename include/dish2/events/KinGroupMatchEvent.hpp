#pragma once
#ifndef DISH2_EVENTS_KINGROUPMATCHEVENT_HPP_INCLUDE
#define DISH2_EVENTS_KINGROUPMATCHEVENT_HPP_INCLUDE

#include <utility>

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Is this cell part of the same kin group as the cardinal's neighbor?
 *
 * Triggered if a cell's kin group ID(s) are equal to the cardinal's neighbors'
 * current kin group ID(s).
 */
struct KinGroupMatchEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    using spec_t = typename Cardinal::spec_t;
    return cardinal.peripheral.readable_state.template Get<
      typename dish2::KinGroupMatch<spec_t>
    >().template Get<
      SeriesIdx
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_KINGROUPMATCHEVENT_HPP_INCLUDE
