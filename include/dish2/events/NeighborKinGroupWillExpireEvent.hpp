#pragma once
#ifndef DISH2_EVENTS_NEIGHBORKINGROUPWILLEXPIREEVENT_HPP_INCLUDE
#define DISH2_EVENTS_NEIGHBORKINGROUPWILLEXPIREEVENT_HPP_INCLUDE

#include <utility>

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Triggered if the cardinal's cell neighbor's kin group age is less than or
 * equal to 80% of the kin group expiration duration.
 */
struct NeighborKinGroupWillExpireEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    using spec_t = typename Cardinal::spec_t;
    return cardinal.peripheral.readable_state.template Get<
      typename dish2::NeighborKinGroupWillExpire<spec_t>
    >().template Get<
      SeriesIdx
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_NEIGHBORKINGROUPWILLEXPIREEVENT_HPP_INCLUDE
