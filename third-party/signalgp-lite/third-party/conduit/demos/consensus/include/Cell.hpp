#pragma once

#include <memory>

#include "uitsl/algorithm/get_plurality.hpp"

#include "typedef.hpp"

class Cell {

  size_t received_message_counter{};
  size_t sent_message_counter{};

  using inputs_t = node_t::inputs_t;
  inputs_t inputs;

  using outputs_t = node_t::outputs_t;
  outputs_t outputs;

  size_t node_id;
  const size_t num_nodes;

  size_t state;

  void PullInputs() {
    std::vector<size_t> values;
    values.reserve( inputs.size() );
    std::transform(
      std::begin(inputs),
      std::end(inputs),
      std::back_inserter(values),
      [this](auto& input){
        received_message_counter += input.Jump();
        return input.Get();
      }
    );

    const size_t plurality = uitsl::get_plurality(
      std::begin(values),
      std::end(values)
    );

    // ignore value-initialized initial Gets
    if (plurality) state = plurality;

  }

  void PushOutputs() {
    for (auto& out : outputs) {
      out.Put( state );
      out.TryFlush();
      ++sent_message_counter;
    }
  }

public:

  Cell(const node_t& node, const size_t num_nodes_)
  : inputs( node.GetInputs() )
  , outputs( node.GetOutputs() )
  , node_id( node.GetNodeID() )
  , num_nodes( num_nodes_ )
  , state( node_id + 1 )
  { }

  void Update() {
    PullInputs();
    PushOutputs();
  }

  size_t GetNumMessagesSent() const { return sent_message_counter; }

  size_t GetNumMessagesReceived() const { return received_message_counter; }

  const size_t GetNodeID() const { return node_id; }

  double GetState() const { return state; }

};
