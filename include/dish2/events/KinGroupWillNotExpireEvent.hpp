#pragma once
#ifndef DISH2_EVENTS_KINGROUPWILLNOTEXPIREEVENT_HPP_INCLUDE
#define DISH2_EVENTS_KINGROUPWILLNOTEXPIREEVENT_HPP_INCLUDE


#include "KinGroupWillExpireEvent.hpp"

namespace dish2 {

/**
 * Triggered if kin group age is less than or equal to than 80\% of the kin
 * group expiration duration.
 */
struct KinGroupWillNotExpireEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( const Cardinal& cardinal ) {
    return !dish2::KinGroupWillExpireEvent::Test<Cardinal, SeriesIdx>(
      cardinal
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_KINGROUPWILLNOTEXPIREEVENT_HPP_INCLUDE
