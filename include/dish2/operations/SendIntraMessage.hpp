#pragma once
#ifndef DISH2_OPERATIONS_SENDINTRAMESSAGE_HPP_INCLUDE
#define DISH2_OPERATIONS_SENDINTRAMESSAGE_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

struct SendIntraMessage {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) {

    auto& outputs = peripheral.intra_message_node_outputs;

    const size_t num_addrs = outputs.size();
    const size_t addr = std::accumulate(
      std::begin( inst.args ),
      std::end( inst.args ),
      0
    ) % num_addrs;

    outputs[ addr ].TryPut( inst.tag );

  }

  static std::string name() { return "SendIntraMessage"; }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_SENDINTRAMESSAGE_HPP_INCLUDE
