#pragma once

#include <memory>

#include "typedef.hpp"

class Cell {

  double mass{};

  size_t received_message_counter{};
  size_t sent_message_counter{};

  using inputs_t = node_t::inputs_t;
  inputs_t inputs;

  using outputs_t = node_t::outputs_t;
  outputs_t outputs;

  size_t node_id;
  const size_t num_nodes;

  static constexpr double diffusion_rate{ 0.5 };

  void PullInputs() {
    for (auto& in : inputs) {
      received_message_counter += in.Jump();
      mass += in.Get();
    }
  }

  void PushOutputs() {
    const double loss{ diffusion_rate * mass };
    mass -= loss;

    const double loss_per{ loss / outputs.size() };

    for (auto& out : outputs) {
      ++sent_message_counter;
      out.Put( loss_per );
      out.TryFlush();
    }
  }

  void FixMass() {
    if (node_id == 0) mass = 1.0;
    else if (node_id + 1 == num_nodes) mass = 0.0;
  }

public:

  Cell(const node_t& node, const size_t num_nodes_)
  : inputs( node.GetInputs() )
  , outputs( node.GetOutputs() )
  , node_id( node.GetNodeID() )
  , num_nodes( num_nodes_ )
  { FixMass(); }

  void Update() {
    PushOutputs();
    PullInputs();
    FixMass();
  }

  size_t GetNumMessagesSent() const { return sent_message_counter; }

  size_t GetNumMessagesReceived() const { return received_message_counter; }

  const size_t GetNodeID() const { return node_id; }

  double GetMass() const { return mass; }

};
