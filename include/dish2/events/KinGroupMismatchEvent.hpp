#pragma once
#ifndef DISH2_EVENTS_KINGROUPMISMATCHEVENT_HPP_INCLUDE
#define DISH2_EVENTS_KINGROUPMISMATCHEVENT_HPP_INCLUDE

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

template<typename Spec>
struct KinGroupMismatchEvent {

  template< typename Cardinal, size_t SeriesIdx >
  static bool Test( Cardinal& cardinal ) {
    return cardinal.peripheral.state_node_input.Get().template Get<
      typename dish2::KinGroupIDView<Spec>
    >().template Get<
      SeriesIdx
    >() != cardinal.peripheral.readable_state.template Get<
      typename dish2::KinGroupIDView<Spec>
    >().template Get<
      SeriesIdx
    >();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_KINGROUPMISMATCHEVENT_HPP_INCLUDE
