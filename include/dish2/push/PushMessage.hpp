#pragma once
#ifndef DISH2_PUSH_PUSHMESSAGE_HPP_INCLUDE
#define DISH2_PUSH_PUSHMESSAGE_HPP_INCLUDE

#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include "../../../third-party/cereal/include/cereal/cereal.hpp"
#include "../../../third-party/cereal/include/cereal/types/variant.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "DistanceToGraphCenterMessage.hpp"
#include "PushCellState.hpp"

namespace dish2 {

class PushMessage {

  std::variant< dish2::DistanceToGraphCenterMessage > data;

public:

  template< typename... Args>
  PushMessage( Args&&... args ) : data( std::forward<Args>( args )... ) {}

  emp::optional<PushMessage> ApplyTo(dish2::PushCellState& state) const {

    using res_t = emp::optional<PushMessage>;

    return std::visit([&state](const auto& message) -> res_t {
      using message_t = std::decay_t< decltype( message ) >;
      using message_state_t = typename message_t::state_t;

      auto& substate = std::get<message_state_t>( state );

      const auto res = message.ApplyTo( substate );

      return res.has_value() ? res_t{ PushMessage{*res} } : std::nullopt;

    }, data);

  }

  template <class Archive>
  void serialize( Archive & ar ) { ar(
    CEREAL_NVP( data )
  ); }

};

} // namespace dish2

#endif // #ifndef DISH2_PUSH_PUSHMESSAGE_HPP_INCLUDE
