#pragma once
#ifndef DISH2_PUSH_DISTANCETOGRAPHCENTERMESSAGE_HPP_INCLUDE
#define DISH2_PUSH_DISTANCETOGRAPHCENTERMESSAGE_HPP_INCLUDE

#include <tuple>

#include "../../../third-party/cereal/include/cereal/cereal.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "DistanceToGraphCenterCellState.hpp"

namespace dish2 {

struct DistanceToGraphCenterMessage {

  size_t sender_distance;

  using this_t = DistanceToGraphCenterMessage;
  using state_t = dish2::DistanceToGraphCenterCellState;

  emp::optional<this_t> ApplyTo( state_t& state ) const {

    const size_t proposed_distance = sender_distance + 1;

    // if we've found a lower distance to center, let neighbors know
    if (
      !state.my_distance.has_value()
      || proposed_distance < state.my_distance
    ) {
      state.my_distance = proposed_distance;
      return { DistanceToGraphCenterMessage{ proposed_distance } };
    } else return std::nullopt;

  }

  template <class Archive>
  void serialize( Archive & ar ) { ar(
    CEREAL_NVP( sender_distance )
  ); }


};

} // namespace dish2

#endif // #ifndef DISH2_PUSH_DISTANCETOGRAPHCENTERMESSAGE_HPP_INCLUDE
