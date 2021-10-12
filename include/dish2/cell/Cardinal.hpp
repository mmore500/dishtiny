#pragma once
#ifndef DISH2_CELL_CARDINAL_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_HPP_INCLUDE

#include <utility>

#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeInput.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeOutput.hpp"
#include "../../../third-party/Empirical/include/emp/bits/BitSet.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../peripheral/Peripheral.hpp"
#include "../quorum/CardinalQuorumState.hpp"

namespace dish2 {

template<typename Spec>
struct Cardinal {

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  using genome_node_input_t = netuit::MeshNodeInput<genome_mesh_spec_t>;
  using genome_node_output_t = netuit::MeshNodeOutput<genome_mesh_spec_t>;
  genome_node_input_t genome_node_input;
  genome_node_output_t genome_node_output;

  using intra_message_mesh_spec_t = typename Spec::intra_message_mesh_spec_t;
  using intra_message_node_t = netuit::MeshNode<intra_message_mesh_spec_t>;
  intra_message_node_t intra_message_node;

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_node_input_t = netuit::MeshNodeInput<message_mesh_spec_t>;
  using message_node_output_t = netuit::MeshNodeOutput<message_mesh_spec_t>;
  message_node_input_t message_node_input;
  message_node_output_t message_node_output;

  using push_mesh_spec_t = typename Spec::push_mesh_spec_t;
  using push_node_input_t = netuit::MeshNodeInput<push_mesh_spec_t>;
  using push_node_output_t = netuit::MeshNodeOutput<push_mesh_spec_t>;
  push_node_input_t push_node_input;
  push_node_output_t push_node_output;

  using quorum_mesh_spec_t = typename Spec::quorum_mesh_spec_t;
  using quorum_node_input_t = netuit::MeshNodeInput<quorum_mesh_spec_t>;
  using quorum_node_output_t = netuit::MeshNodeOutput<quorum_mesh_spec_t>;

  dish2::CardinalQuorumState< Spec > cardinal_quorum_state;

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using resource_node_input_t = netuit::MeshNodeInput<resource_mesh_spec_t>;
  using resource_node_output_t = netuit::MeshNodeOutput<resource_mesh_spec_t>;
  resource_node_input_t resource_node_input;
  resource_node_output_t resource_node_output;

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  using state_node_input_t = netuit::MeshNodeInput<state_mesh_spec_t>;
  using state_node_output_t = netuit::MeshNodeOutput<state_mesh_spec_t>;
  state_node_input_t state_node_input;
  state_node_output_t state_node_output;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  using tag_t = typename Spec::tag_t;

  using cpu_t = sgpl::Cpu<sgpl_spec_t>;
  cpu_t cpu;

  using peripheral_t = dish2::Peripheral<Spec>;
  peripheral_t peripheral;

  using spec_t = Spec;

  Cardinal(
    const genome_node_input_t& genome_node_input_,
    const genome_node_output_t& genome_node_output_,
    const message_node_input_t& message_node_input_,
    const message_node_output_t& message_node_output_,
    const push_node_input_t& push_node_input_,
    const push_node_output_t& push_node_output_,
    const quorum_node_input_t& quorum_node_input_,
    const quorum_node_output_t& quorum_node_output_,
    const resource_node_input_t& resource_node_input_,
    const resource_node_output_t& resource_node_output_,
    const state_node_input_t& state_node_input_,
    const state_node_output_t& state_node_output_,
    const intra_message_node_t& intra_message_node_
  ) :
    genome_node_input( genome_node_input_ )
  , genome_node_output( genome_node_output_ )
  , intra_message_node( intra_message_node_ )
  , message_node_input( message_node_input_ )
  , message_node_output( message_node_output_ )
  , push_node_input( push_node_input_ )
  , push_node_output( push_node_output_ )
  , cardinal_quorum_state( quorum_node_input_, quorum_node_output_ )
  , resource_node_input( resource_node_input_ )
  , resource_node_output( resource_node_output_ )
  , state_node_input( state_node_input_ )
  , state_node_output( state_node_output_ )
  , peripheral(
    intra_message_node.GetOutputs(), message_node_output, state_node_input
  )
  {}

  void LoadProgram(
    const sgpl::Program<sgpl_spec_t>& program, const size_t root_id
  ) {
    cpu.InitializeAnchors( program );
    peripheral.root_id = root_id;
  }

  void Reset() { cpu.Reset(); peripheral.Clear(); }

  void DispatchEvent(const tag_t& tag) { cpu.ForceLaunchCore( tag ); }

  bool IsNeighborKin( const size_t lev ) {

    return peripheral.readable_state.template Get<
      dish2::KinGroupIDView< Spec >
    >().Get( lev ) == std::as_const(state_node_input).Get().template Get<
      dish2::KinGroupIDView< Spec >
    >().Get( lev );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_HPP_INCLUDE
