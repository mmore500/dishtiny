#pragma once
#ifndef DISH2_CELL_CELL_HPP_INCLUDE
#define DISH2_CELL_CELL_HPP_INCLUDE

#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"
#include "../../../third-party/Empirical/source/base/vector.h"

#include "../genome/Genome.hpp"

#include "Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class Cell {

  emp::vector< dish2::Cardinal<Spec> > cardinals;

  dish2::Genome<Spec> genome;

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  using genome_node_t = netuit::MeshNode<genome_mesh_spec_t>;

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_node_t = netuit::MeshNode<message_mesh_spec_t>;

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using resource_node_t = netuit::MeshNode<resource_mesh_spec_t>;

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  using state_node_t = netuit::MeshNode<state_mesh_spec_t>;

public:

  Cell(
    const genome_node_t& genome_node,
    const message_node_t& message_node,
    const resource_node_t& resource_node,
    const state_node_t& state_node
  ) {

    emp_assert(( 1 == std::set<size_t>{
      genome_node.GetNumInputs(),
      message_node.GetNumInputs(),
      resource_node.GetNumInputs(),
      state_node.GetNumInputs(),
      genome_node.GetNumOutputs(),
      message_node.GetNumOutputs(),
      resource_node.GetNumOutputs(),
      state_node.GetNumOutputs(),
    }.size() ));

    for (size_t i{}; i < message_node.GetNumInputs(); ++i) {
      cardinals.emplace_back(
        genome_node.GetInput(i),
        genome_node.GetOutput(i),
        message_node.GetInput(i),
        message_node.GetOutput(i),
        resource_node.GetInput(i),
        resource_node.GetOutput(i),
        state_node.GetInput(i),
        state_node.GetOutput(i)
      );
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CELL_HPP_INCLUDE
