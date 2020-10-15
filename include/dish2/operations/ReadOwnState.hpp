#pragma once
#ifndef DISH2_OPERATIONS_READOWNSTATE_HPP_INCLUDE
#define DISH2_OPERATIONS_READOWNSTATE_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

struct ReadOwnState {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    const typename Spec::peripheral_t& peripheral
  ) {

    constexpr size_t num_addrs
      = decltype(peripheral.readable_state)::GetSize();
    const size_t addr = inst.tag.GetUInt(0) % num_addrs;

    core.registers[ inst.args[0] ] = peripheral.readable_state.Read(addr);

  }

  static std::string name() { return "ReadOwnState"; }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_READOWNSTATE_HPP_INCLUDE
