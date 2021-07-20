#pragma once
#ifndef DISH2_EVENTS_ISCHILDGROUPOFEVENT_HPP_INCLUDE
#define DISH2_EVENTS_ISCHILDGROUPOFEVENT_HPP_INCLUDE

#include <utility>

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Is this cell's kin group a daughter group of the cardinal's neighbor cell's
 * kin group?
 *
 * Triggered if a cell's kin group ancestor ID(s) are equal to the cardinal's
 * neighbor's current kin group ID(s).
 */
struct IsChildGroupOfEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    using spec_t = typename Cardinal::spec_t;
    return cardinal.peripheral.readable_state.template Get<
      typename dish2::IsChildGroupOf<spec_t>
    >().template Get<
      SeriesIdx
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_ISCHILDGROUPOFEVENT_HPP_INCLUDE
