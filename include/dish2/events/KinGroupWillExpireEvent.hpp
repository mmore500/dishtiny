#pragma once
#ifndef DISH2_EVENTS_KINGROUPWILLEXPIREEVENT_HPP_INCLUDE
#define DISH2_EVENTS_KINGROUPWILLEXPIREEVENT_HPP_INCLUDE

#include <utility>

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Triggered if kin group age is greater than 80\% of the kin group expiration
 * duration.
 *
 * (Depending on experiment configuration, the group may be force-fragmented
 * after expiration.)
 */
struct KinGroupWillExpireEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    using spec_t = typename Cardinal::spec_t;
    return cardinal.peripheral.readable_state.template Get<
      typename dish2::KinGroupWillExpire<spec_t>
    >().template Get<
      SeriesIdx
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_KINGROUPWILLEXPIREEVENT_HPP_INCLUDE
