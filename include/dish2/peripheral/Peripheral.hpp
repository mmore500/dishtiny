#pragma once
#ifndef DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE
#define DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE

#include <deque>

#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeInput.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeOutput.hpp"
#include "../../../third-party/conduit/include/uit/fixtures/Sink.hpp"
#include "../../../third-party/conduit/include/uit/fixtures/Source.hpp"

#include "../spec/IntraMessageMeshSpec.hpp"
#include "../spec/MessageMeshSpec.hpp"
#include "../spec/StateMeshSpec.hpp"

#include "readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * State that SignalGP-Lite operations can interact with.
 */
template<typename Spec>
struct Peripheral {

  dish2::ReadableState<Spec> readable_state{};

  using intra_message_mesh_spec_t = dish2::IntraMessageMeshSpec< Spec >;
  using intra_message_node_outputs_t
    = typename netuit::MeshNode<intra_message_mesh_spec_t>::outputs_t;
  intra_message_node_outputs_t intra_message_node_outputs;

  using message_mesh_spec_t = dish2::MessageMeshSpec< Spec >;
  using message_node_output_t = netuit::MeshNodeOutput<message_mesh_spec_t>;
  message_node_output_t message_node_output;

  using state_mesh_spec_t = dish2::StateMeshSpec<Spec>;
  using state_node_input_t = netuit::MeshNodeInput<state_mesh_spec_t>;
  state_node_input_t state_node_input;

  size_t root_id;

  std::deque<typename Spec::message_t> inter_message_selfsend_buffer;
  std::deque<typename Spec::message_t> intra_message_selfsend_buffer;

  static Peripheral make_dummy() {
    intra_message_node_outputs_t intra_message_node_outputs{};
    message_node_output_t message_node_output{
      uit::Sink<message_mesh_spec_t>{}.GetInlet(),
      0
    };
    state_node_input_t state_node_input{
      uit::Source<state_mesh_spec_t>{}.GetOutlet(),
      0
    };
    return Peripheral(
      intra_message_node_outputs,
      message_node_output,
      state_node_input
    );
  }

  Peripheral(
    intra_message_node_outputs_t& intra_message_node_outputs_,
    message_node_output_t& message_node_output_,
    state_node_input_t& state_node_input_
  ) : intra_message_node_outputs( intra_message_node_outputs_ )
  , message_node_output( message_node_output_ )
  , state_node_input( state_node_input_ )
  {}

  bool operator==(const Peripheral& other) const {
    return readable_state == other.readable_state;
  }

  void Clear() { readable_state.Reset(); }

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE
