#pragma once
#ifndef DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE
#define DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE

#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeInput.hpp"

#include "../spec/StateMeshSpec.hpp"

#include "readable_state/ReadableState.hpp"

namespace dish2 {

struct Peripheral {

  dish2::ReadableState readable_state{};

  using state_mesh_spec_t = dish2::StateMeshSpec;
  using state_node_input_t = netuit::MeshNodeInput<state_mesh_spec_t>;

  void RefreshInput() { neighbor_state.Jump(); }

  state_node_input_t neighbor_state;

  explicit Peripheral(
    const state_node_input_t& neighbor_state_
  ) : neighbor_state(neighbor_state_)
  {}

  bool operator==(const Peripheral& other) const {
    return readable_state == other.readable_state;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_PERIPHERAL_HPP_INCLUDE
