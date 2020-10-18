#pragma once
#ifndef DISH2_OPERATIONS_READNEIGHBORSTATE_HPP_INCLUDE
#define DISH2_OPERATIONS_READNEIGHBORSTATE_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

struct ReadNeighborState {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) {

    const auto& neighbor_state = peripheral.state_node_input.Get();

    constexpr size_t num_addrs
      = decltype(peripheral.readable_state)::GetSize();
    const size_t addr = inst.tag.GetUInt(0) % num_addrs;

    core.registers[ inst.args[0] ] = neighbor_state.Read(addr);

  }

  static std::string name() { return "ReadNeighborState"; }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_READNEIGHBORSTATE_HPP_INCLUDE
