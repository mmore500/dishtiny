#pragma once
#ifndef DISH2_CELL_CARDINAL_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_HPP_INCLUDE

#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeInput.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeOutput.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../peripheral/Peripheral.hpp"

namespace dish2 {

template<typename Spec>
struct Cardinal {

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  using genome_node_input_t = netuit::MeshNodeInput<genome_mesh_spec_t>;
  using genome_node_output_t = netuit::MeshNodeOutput<genome_mesh_spec_t>;
  genome_node_input_t genome_node_input;
  genome_node_output_t genome_node_output;

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_node_input_t = netuit::MeshNodeInput<message_mesh_spec_t>;
  using message_node_output_t = netuit::MeshNodeOutput<message_mesh_spec_t>;
  message_node_input_t message_node_input;
  message_node_output_t message_node_output;

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

  using cpu_t = sgpl::Cpu<sgpl_spec_t>;
  cpu_t cpu;

  using peripheral_t = dish2::Peripheral;
  peripheral_t peripheral;

  Cardinal(
    const genome_node_input_t& genome_node_input_,
    const genome_node_output_t& genome_node_output_,
    const message_node_input_t& message_node_input_,
    const message_node_output_t& message_node_output_,
    const resource_node_input_t& resource_node_input_,
    const resource_node_output_t& resource_node_output_,
    const state_node_input_t& state_node_input_,
    const state_node_output_t& state_node_output_
  ) :
    genome_node_input( genome_node_input_ )
  , genome_node_output( genome_node_output_ )
  , message_node_input( message_node_input_ )
  , message_node_output( message_node_output_ )
  , resource_node_input( resource_node_input_ )
  , resource_node_output( resource_node_output_ )
  , state_node_input( state_node_input_ )
  , state_node_output( state_node_output_ )
  , peripheral( message_node_output, state_node_input )
  {
    ; // TODO set peripheral to channel IDs
  }

  void LoadProgram(const sgpl::Program<sgpl_spec_t>& program) {
    cpu.InitializeAnchors( program );
  }

  void Reset() { cpu.Reset(); peripheral.Clear(); }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_HPP_INCLUDE
