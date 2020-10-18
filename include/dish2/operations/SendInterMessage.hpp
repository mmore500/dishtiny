#pragma once
#ifndef DISH2_OPERATIONS_SENDINTERMESSAGE_HPP_INCLUDE
#define DISH2_OPERATIONS_SENDINTERMESSAGE_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

struct SendInterMessage {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) {

    peripheral.message_node_output.TryPut( inst.tag );

  }

  static std::string name() { return "SendInterMessage"; }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_SENDINTERMESSAGE_HPP_INCLUDE
