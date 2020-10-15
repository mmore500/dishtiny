#pragma once
#ifndef DISH2_OPERATIONS_WRITEOWNSTATE_HPP_INCLUDE
#define DISH2_OPERATIONS_WRITEOWNSTATE_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../peripheral/readable_state/writable_state/WritableState.hpp"

namespace dish2 {

struct WriteOwnState {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) {

    constexpr size_t num_addrs
      = decltype(peripheral.readable_state)::GetSize();
    const size_t addr = inst.tag.GetUInt(0) % num_addrs;

    peripheral.readable_state.template Get<dish2::WritableState>().Write(
      addr,
      core.registers[ inst.args[0] ]
    );

  }

  static std::string name() { return "WriteOwnState"; }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_WRITEOWNSTATE_HPP_INCLUDE
