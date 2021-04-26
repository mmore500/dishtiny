#pragma once

#include <memory>

#include "../../../third-party/Empirical/include/emp/hardware/signalgp_utils.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"

#include "config.hpp"
#include "EventLib.hpp"
#include "InstLib.hpp"
#include "Trait.hpp"
#include "typedef.hpp"

class Cell {

  emp::Random random{ 1 };

  hardware_t hardware{
    &get_inst_lib(),
    &get_event_lib(),
    &random
  };

  using inputs_t = node_t::inputs_t;
  inputs_t inputs;

  size_t node_id;

  void Compute() { for (size_t i = 0; i < 100; ++i) hardware.SingleProcess(); }

  void FetchAndQueueMessages() {
    for (auto& input : inputs) {
      for (
        auto msg = input.GetNextOrNullopt();
        msg.has_value();
        msg = input.GetNextOrNullopt()
      ) hardware.QueueEvent(*msg);
    }
  }

  void FlushOutputs() { hardware.GetTrait()->FlushOutputs(); }

  void SpawnCore() { hardware.SpawnCore(0); }

public:

  Cell(const node_t& node)
  : inputs( node.GetInputs() )
  , node_id( node.GetNodeID() )
  {
    hardware.SetTrait( std::make_unique<Trait>(node.GetOutputs()) );
    hardware.SetProgram( emp::GenRandSignalGPProgram<TAG_WIDTH, trait_t>(
      random,
      get_inst_lib(),
      PROGRAM_MIN_FUN_CNT__INIT, PROGRAM_MAX_FUN_CNT__INIT,
      PROGRAM_MIN_FUN_LEN__INIT, PROGRAM_MAX_FUN_LEN__INIT,
      PROGRAM_MIN_ARG_VAL__INIT, PROGRAM_MAX_ARG_VAL__INIT
    ) );
  }


  void Update() {
    FetchAndQueueMessages();
    SpawnCore();
    Compute();
    FlushOutputs();
  }

  size_t GetNumMessagesSent() const {
    return hardware.GetTrait()->GetNumMessagesSent();
  }

  size_t GetNumMessagesReceived() const {
    return hardware.GetTrait()->GetNumMessagesReceived();
  }

  const size_t GetNodeID() const { return node_id; }

};
