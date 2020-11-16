#pragma once
#ifndef DISH2_OPERATIONS_SENDINTERMESSAGE_HPP_INCLUDE
#define DISH2_OPERATIONS_SENDINTERMESSAGE_HPP_INCLUDE

#include <tuple>

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

    peripheral.message_node_output.TryPut( std::make_tuple(
      inst.tag, core.GetRegisters()
    ) );

  }

  static std::string name() { return "SendInterMessage"; }

  static size_t prevalence() { return 5; }

  static size_t num_registers_to_print() { return 0; }

  static bool should_print_tag() { return true; }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_SENDINTERMESSAGE_HPP_INCLUDE
