
#ifndef EMP_SIGNALGP_LINEAR_FUNCTIONS_PROGRAM_INST_IMPL_H
#define EMP_SIGNALGP_LINEAR_FUNCTIONS_PROGRAM_INST_IMPL_H

#include <iostream>
#include <utility>

#include "emp/base/Ptr.hpp"
#include "emp/base/vector.hpp"
#include "emp/math/Random.hpp"
#include "emp/matching/MatchBin.hpp"
#include "emp/matching/matchbin_utils.hpp"

#include "../EventLibrary.h"
#include "InstructionLibrary.h"

namespace sgp { namespace lfp_inst_impl {

  // These instructions work with:
  // - LinearFunctionsProgram<Tags=BitSet, Args=int>
  // - SimpleMemoryModel

  // Instructions to implement:
  // - Inst_If
  // - Inst_While
  // - Inst_Countdown
  // - Inst_Routine

  // - Inst_If TODO - TEST EDGE CASES!
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_If(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    size_t cur_ip = call_state.GetIP();
    const size_t cur_mp = call_state.GetMP();
    emp_assert(cur_ip > 0);
    // CurIP is the next instruction (not the one currently executing)
    // Because IP gets incremented before execution, cur_ip should never be 0.
    const size_t bob = cur_ip - 1; // beginning of block
    const size_t eob = hw.FindEndOfBlock(cur_mp, cur_ip); // end of block
    const bool skip = !((bool)mem_state.AccessWorking(inst.GetArg(0)));
    if (skip) {
      // Skip to EOB
      call_state.SetIP(eob);
      // Advance past the block close if not at end of module.
      if (hw.IsValidProgramPosition(cur_mp, eob)) {
        ++call_state.IP();
      }
    } else {
      // Open flow
      emp_assert(cur_mp < hw.GetProgram().GetSize());
      hw.GetFlowHandler().OpenFlow(hw,
                                   {lsgp_utils::FlowType::BASIC,
                                    cur_mp,
                                    cur_ip,
                                    bob,
                                    eob
                                   },
                                   hw.GetCurThread().GetExecState());
    }
  }

  // - Inst_While
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_While(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    size_t cur_ip = call_state.GetIP();
    const size_t cur_mp = call_state.GetMP();
    emp_assert(cur_ip > 0);
    // CurIP is the next instruction (not the one currently executing)
    // Because IP gets incremented before execution, cur_ip should never be 0.
    const size_t bob = cur_ip - 1;
    const size_t eob = hw.FindEndOfBlock(cur_mp, cur_ip);
    const bool skip = !((bool)mem_state.AccessWorking(inst.GetArg(0)));
    if (skip) {
      // Skip to EOB
      call_state.SetIP(eob);
      // Advance past the block close if not at end of module.
      if (hw.IsValidProgramPosition(cur_mp, eob)) {
        ++call_state.IP();
      }
    } else {
      // Open flow
      emp_assert(cur_mp < hw.GetProgram().GetSize());
      hw.GetFlowHandler().OpenFlow(hw,{lsgp_utils::FlowType::WHILE_LOOP,
                                       cur_mp,
                                       cur_ip,
                                       bob,
                                       eob},
                                       hw.GetCurThread().GetExecState());
    }
  }

  // - Inst_Countdown
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Countdown(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    size_t cur_ip = call_state.GetIP();
    const size_t cur_mp = call_state.GetMP();
    emp_assert(cur_ip > 0);
    // CurIP is the next instruction (not the one currently executing)
    // Because IP gets incremented before execution, cur_ip should never be 0.
    const size_t bob = cur_ip - 1;
    const size_t eob = hw.FindEndOfBlock(cur_mp, cur_ip);
    const bool skip = mem_state.AccessWorking(inst.GetArg(0)) <= 0;
    if (skip) {
      // Skip to EOB
      call_state.SetIP(eob);
      // Advance past the block close if not at end of module.
      if (hw.IsValidProgramPosition(cur_mp, eob)) {
        ++call_state.IP();
      }
    } else {
      --mem_state.AccessWorking(inst.args[0]);
      // Open flow
      emp_assert(cur_mp < hw.GetProgram().GetSize());
      hw.GetFlowHandler().OpenFlow(hw,{lsgp_utils::FlowType::WHILE_LOOP,
                                       cur_mp,
                                       cur_ip,
                                       bob,
                                       eob},
                                       hw.GetCurThread().GetExecState());
    }
  }

  // - Inst_Routine
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Routine(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    using flow_type_t = lsgp_utils::FlowType;
    emp::vector<size_t> matches(hw.FindModuleMatch(inst.GetTag(0)));
    if (matches.size()) {
      const size_t module_id = matches[0];
      emp_assert(module_id < hw.GetProgram().GetSize());
      const auto & target_module = hw.GetProgram()[module_id];
      // Flow: type mp ip begin end
      hw.GetFlowHandler().OpenFlow(hw, {flow_type_t::ROUTINE,
                                    module_id,
                                    0,
                                    0,
                                    target_module.GetSize()},
                                    hw.GetCurThread().GetExecState());
    }
  }

}}

#endif
