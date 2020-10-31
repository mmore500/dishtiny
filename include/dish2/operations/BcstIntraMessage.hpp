#pragma once
#ifndef DISH2_OPERATIONS_BCSTINTRAMESSAGE_HPP_INCLUDE
#define DISH2_OPERATIONS_BCSTINTRAMESSAGE_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

struct BcstIntraMessage {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) {

    for ( auto& out : peripheral.intra_message_node_outputs ) {
      out.TryPut( inst.tag );
    }

  }

  static std::string name() { return "BcstIntraMessage"; }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_BCSTINTRAMESSAGE_HPP_INCLUDE
