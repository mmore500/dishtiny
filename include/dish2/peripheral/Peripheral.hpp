#pragma once
#ifndef DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE
#define DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE

#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeInput.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeOutput.hpp"

#include "../spec/MessageMeshSpec.hpp"
#include "../spec/StateMeshSpec.hpp"

#include "readable_state/introspective_state/IsAlive.hpp"
#include "readable_state/ReadableState.hpp"

namespace dish2 {

struct Peripheral {

  dish2::ReadableState readable_state{};

  using message_mesh_spec_t = dish2::MessageMeshSpec;
  using message_node_output_t = netuit::MeshNodeOutput<message_mesh_spec_t>;
  message_node_output_t message_node_output;

  using state_mesh_spec_t = dish2::StateMeshSpec;
  using state_node_input_t = netuit::MeshNodeInput<state_mesh_spec_t>;
  state_node_input_t state_node_input;

  Peripheral(
    message_node_output_t& message_node_output_,
    state_node_input_t& state_node_input_
  ) : message_node_output( message_node_output_ )
  , state_node_input( state_node_input_ )
  {}

  bool operator==(const Peripheral& other) const {
    return readable_state == other.readable_state;
  }

  void Clear() { readable_state.Reset(); }

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE
