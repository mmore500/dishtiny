#pragma once
#ifndef DISH2_OPERATIONS_ADDTOOWNSTATE_HPP_INCLUDE
#define DISH2_OPERATIONS_ADDTOOWNSTATE_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../peripheral/readable_state/writable_state/WritableState.hpp"

namespace dish2 {

template< typename DishSpec >
struct AddToOwnState {

  template<typename SgplSpec>
  static void run(
    sgpl::Core<SgplSpec>& core,
    const sgpl::Instruction<SgplSpec>& inst,
    const sgpl::Program<SgplSpec>&,
    typename SgplSpec::peripheral_t& peripheral
  ) {

    constexpr size_t num_addrs = dish2::WritableState< DishSpec >::GetSize();
    const size_t addr = inst.tag.GetUInt(0) % num_addrs;

    peripheral.readable_state.template Get<
      dish2::WritableState< DishSpec >
    >().AddTo(
      addr,
      core.registers[ inst.args[0] ]
    );

  }

  static std::string name() { return "AddToOwnState"; }

  static size_t prevalence() { return 5; }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_ADDTOOWNSTATE_HPP_INCLUDE
