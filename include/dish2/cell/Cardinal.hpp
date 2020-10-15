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

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_node_input_t = netuit::MeshNodeInput<message_mesh_spec_t>;
  using message_node_output_t = netuit::MeshNodeOutput<message_mesh_spec_t>;

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using resource_node_input_t = netuit::MeshNodeInput<resource_mesh_spec_t>;
  using resource_node_output_t = netuit::MeshNodeOutput<resource_mesh_spec_t>;

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  using state_node_input_t = netuit::MeshNodeInput<state_mesh_spec_t>;
  using state_node_output_t = netuit::MeshNodeOutput<state_mesh_spec_t>;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  sgpl::Cpu<sgpl_spec_t> cpu;

  dish2::Peripheral peripheral;

  Cardinal(
    const message_node_input_t& message_node_input,
    const message_node_output_t& message_node_output,
    const resource_node_input_t& resource_node_input,
    const resource_node_output_t& resource_node_output,
    const state_node_input_t& state_node_input,
    const state_node_output_t& state_node_output
  ) : peripheral(state_node_input)
  {
    ;
  }

  void LoadProgram(const sgpl::Program<sgpl_spec_t>& program) {
    cpu.InitializeAnchors( program );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_HPP_INCLUDE
