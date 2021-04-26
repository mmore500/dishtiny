
#ifndef EMP_SIGNALGP_V2_INST_IMPL_H
#define EMP_SIGNALGP_V2_INST_IMPL_H

#include <iostream>
#include <utility>

#include "emp/base/Ptr.hpp"
#include "emp/base/vector.hpp"
#include "emp/math/Random.hpp"
#include "emp/matching/MatchBin.hpp"
#include "emp/matching/matchbin_utils.hpp"

#include "../EventLibrary.h"
#include "InstructionLibrary.h"

// #include "../SignalGP.h"

namespace sgp { namespace inst_impl {

  // These instructions work with:
  // - SimpleExecutionStepper
  //  - LinearProgram
  //  - SimpleMemoryModel

  // NUMERIC ARG INSTRUCTIONS

  // Instructions to implement:
  // - Inst_Inc
  // - Inst_Dec
  // - Inst_Not
  // - Inst_Add
  // - Inst_Sub
  // - Inst_Mult
  // - Inst_Div
  // - Inst_Mod
  // - Inst_TestEqu
  // - Inst_TestNEqu
  // - Inst_TestLess
  // - Inst_If
  // - Inst_While
  // - Inst_Countdown
  // - Inst_Break
  // - Inst_Close
  // - Inst_Call
  // - Inst_Return
  // - Inst_SetMem
  // - Inst_CopyMem
  // - Inst_SwapMem
  // - Inst_Input
  // - Inst_Output
  // - Inst_Commit
  // - Inst_Pull
  // - Inst_Fork
  // - Inst_Terminate
  // - Inst_Nop

  /// Increment
  /// - requires: 1 argument
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Inc(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    // Increment value in local memory @ [ARG0]
    ++call_state.GetMemory().AccessWorking(inst.GetArg(0));
  }

  /// Decrement
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Dec(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    // Decrement value in local memory @ [ARG0]
    --call_state.GetMemory().AccessWorking(inst.GetArg(0));
  }

  /// Not
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Not(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(0), mem_state.GetWorking(inst.GetArg(0)) == 0.0);
  }

  // - Inst_Add
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Add(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) + mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_Sub
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Sub(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) - mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_Mult
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Mult(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) * mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_Div
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Div(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    const auto & denom = mem_state.AccessWorking(inst.GetArg(1));
    if (denom == 0.0) return; // Do nothing.
    const auto & num = mem_state.AccessWorking(inst.GetArg(0));
    mem_state.SetWorking(inst.GetArg(2), num / denom);
  }

  // - Inst_Mod
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Mod(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    const int denom = (int)mem_state.AccessWorking(inst.GetArg(1));
    if (denom == 0.0) return; // Do nothing.
    const int num = (int)mem_state.AccessWorking(inst.GetArg(0));
    mem_state.SetWorking(inst.GetArg(2), static_cast<int64_t>(num) % static_cast<int64_t>(denom));
  }

  // - Inst_TestEqu
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_TestEqu(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) == mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_TestNEqu
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_TestNEqu(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) != mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_TestLess
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_TestLess(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) < mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_TestLessEqu
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_TestLessEqu(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) <= mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_TestGreater
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_TestGreater(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) > mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_TestGreater
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_TestGreaterEqu(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(2), mem_state.AccessWorking(inst.GetArg(0)) >= mem_state.AccessWorking(inst.GetArg(1)));
  }

  // - Inst_If TODO - TEST EDGE CASES!
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_If(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    const size_t prog_len = hw.GetProgram().GetSize();
    size_t cur_ip = call_state.GetIP();
    const size_t cur_mp = call_state.GetMP();
    const auto & module = hw.GetModule(cur_mp);
    const size_t module_begin = module.GetBegin();
    const size_t module_end = module.GetEnd();
    // Beginning of block (if instruction).
    const size_t bob = (cur_ip == 0) ? prog_len - 1 : cur_ip - 1;
    // Find end of flow. ==> PROBLEM: what if 'If' is last instruction
    cur_ip = (cur_ip == prog_len
              && module_begin > module_end
              && module.InModule(0)) ? 0 : cur_ip;
    const size_t eob = hw.FindEndOfBlock(cur_mp, cur_ip); // CurIP is next instruction (not the one currently executing)
    const bool skip = mem_state.AccessWorking(inst.GetArg(0)) == 0.0;
    if (skip) {
      // Skip to EOB
      call_state.SetIP(eob);
      // Advance past the block close if not at end of module.
      if (hw.IsValidProgramPosition(cur_mp, eob)) {
        ++call_state.IP();
      }
    } else {
      // Open flow
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
    const size_t prog_len = hw.GetProgram().GetSize();
    size_t cur_ip = call_state.GetIP();
    const size_t cur_mp = call_state.GetMP();
    const auto & module = hw.GetModule(cur_mp);
    const size_t module_begin = module.GetBegin();
    const size_t module_end = module.GetEnd();
    // Beginning of block (if instruction).
    const size_t bob = (cur_ip == 0) ? prog_len - 1 : cur_ip - 1;
    // Find end of flow. ==> PROBLEM: what if 'If' is last instruction
    cur_ip = (cur_ip == prog_len
              && module_begin > module_end
              && module.InModule(0)) ? 0 : cur_ip;
    const size_t eob = hw.FindEndOfBlock(cur_mp, cur_ip);
    const bool skip = mem_state.AccessWorking(inst.GetArg(0)) == 0.0;
    if (skip) {
      // Skip to EOB
      call_state.SetIP(eob);
      // Advance past the block close if not at end of module.
      if (hw.IsValidProgramPosition(cur_mp, eob)) {
        ++call_state.IP();
      }
    } else {
      // Open flow
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
    const size_t prog_len = hw.GetProgram().GetSize();
    size_t cur_ip = call_state.GetIP();
    const size_t cur_mp = call_state.GetMP();
    const auto & module = hw.GetModule(cur_mp);
    const size_t module_begin = module.GetBegin();
    const size_t module_end = module.GetEnd();
    // Beginning of block (if instruction).
    const size_t bob = (cur_ip == 0) ? prog_len - 1 : cur_ip - 1;
    // Find end of flow. ==> PROBLEM: what if 'If' is last instruction
    cur_ip = (cur_ip == prog_len
              && module_begin > module_end
              && module.InModule(0)) ? 0 : cur_ip;
    const size_t eob = hw.FindEndOfBlock(cur_mp, cur_ip);
    const bool skip = mem_state.AccessWorking(inst.GetArg(0)) == 0.0;
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
      hw.GetFlowHandler().OpenFlow(hw,{lsgp_utils::FlowType::WHILE_LOOP,
                                              cur_mp,
                                              cur_ip,
                                              bob,
                                              eob},
                                              hw.GetCurThread().GetExecState());
    }
  }
  // - Inst_Break
  //   - break out of nearest loop in flow stack (that isn't preceded by a routine or call)
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Break(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    using flow_type_t = lsgp_utils::FlowType;
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    // break out of the nearest loop:
    //     loop = false;
    // (1) While (true) {
    //       if (basic) continue; ++i
    //       elif (loop) loop = true; break;
    //       else break;
    //     }
    bool found_loop = false;
    int flow_pos = call_state.flow_stack.size() - 1;
    while (flow_pos >= 0) {
      auto & flow = call_state.flow_stack[flow_pos];
      if (flow.GetType() == flow_type_t::BASIC) {
        --flow_pos;
        continue;
      } else if (flow.GetType() == flow_type_t::WHILE_LOOP) {
        found_loop = true;
        break;
      } else {
        break;
      }
    }
    // (2) While (loop) {
    //       if (basic) pop_back()
    //       if (loop) {
    //         BreakFlow(loop, exec_state);
    //         break;
    //       }
    //     }
    while (found_loop) {
      if (call_state.GetTopFlow().GetType() == flow_type_t::BASIC) {
        call_state.flow_stack.pop_back(); // todo - CloseFlow?
      } else {
        emp_assert(call_state.GetTopFlow().GetType() == flow_type_t::WHILE_LOOP);
        hw.GetFlowHandler().BreakFlow(hw, flow_type_t::WHILE_LOOP, hw.GetCurThread().GetExecState());
        break;
      }
    }
  }

  // - Inst_Close
  //   - close basic and while_loop flow
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Close(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    using flow_type_t = lsgp_utils::FlowType;
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    const flow_type_t cur_flow_type = call_state.GetTopFlow().GetType();
    if (cur_flow_type == flow_type_t::BASIC || cur_flow_type == flow_type_t::WHILE_LOOP) {
      hw.GetFlowHandler().CloseFlow(hw, cur_flow_type, hw.GetCurThread().GetExecState());
    }
  }

  // - Inst_Call
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Call(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    hw.CallModule(inst.GetTag(0), hw.GetCurThread().GetExecState());
  }

  // - Inst_Routine
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Routine(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    using flow_type_t = lsgp_utils::FlowType;
    emp::vector<size_t> matches(hw.FindModuleMatch(inst.GetTag(0)));
    if (matches.size()) {
      const auto & target_module = hw.GetModule(matches[0]);
      // Flow: type mp ip begin end
      hw.GetFlowHandler().OpenFlow(hw, {flow_type_t::ROUTINE,
                                    target_module.id,
                                    target_module.begin,
                                    target_module.begin,
                                    target_module.end},
                                    hw.GetCurThread().GetExecState());
    }
  }

  // - Inst_Return
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Return(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    using flow_type_t = lsgp_utils::FlowType;
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    // Return from CALL or ROUTINE
    while (call_state.IsFlow()) {
      auto & top = call_state.GetTopFlow();
      if (top.GetType() == flow_type_t::CALL) {
        hw.GetFlowHandler().CloseFlow(hw, top.GetType(), hw.GetCurThread().GetExecState());
        hw.ReturnCall(hw.GetCurThread().GetExecState()); // Actually return from call!
      } else if (top.GetType() == flow_type_t::ROUTINE) {
        hw.GetFlowHandler().CloseFlow(hw, top.GetType(), hw.GetCurThread().GetExecState());
      } else {
        hw.GetFlowHandler().CloseFlow(hw, top.GetType(), hw.GetCurThread().GetExecState());
        continue;
      }
      break;
    }
  }

  // - Inst_SetMem
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_SetMem(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(0), (double)inst.GetArg(1));
  }

  // - Inst_CopyMem
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_CopyMem(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(1), mem_state.AccessWorking(inst.GetArg(0)));
  }

  // - Inst_SwapMem
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_SwapMem(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    const double val_0 = mem_state.AccessWorking(inst.GetArg(0));
    const double val_1 = mem_state.AccessWorking(inst.GetArg(1));
    mem_state.SetWorking(inst.GetArg(0), val_1);
    mem_state.SetWorking(inst.GetArg(1), val_0);
  }

  // - Inst_Input
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_InputToWorking(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetWorking(inst.GetArg(1), mem_state.AccessInput(inst.GetArg(0)));
  }

  // - Inst_Output
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_WorkingToOutput(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    mem_state.SetOutput(inst.GetArg(1), mem_state.AccessWorking(inst.GetArg(0)));
  }

  // - Inst_Commit (push value from working to global memory)
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_WorkingToGlobal(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    auto & mem_model = hw.GetMemoryModel();
    mem_model.SetGlobal(inst.GetArg(1), mem_state.AccessWorking(inst.GetArg(0)));
  }

  // - Inst_Pull (pull value from global to working memory)
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_GlobalToWorking(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    auto & mem_model = hw.GetMemoryModel();
    mem_state.SetWorking(inst.GetArg(1), mem_model.AccessGlobal(inst.GetArg(0)));
  }

  /// Copy full working memory into global memory buffer todo - test
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_FullWorkingToGlobal(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    auto & mem_model = hw.GetMemoryModel();
    auto & working_mem_buffer = mem_state.GetWorkingMemory();
    for (auto & mem : working_mem_buffer) {
      mem_model.SetGlobal(mem.first, mem.second);
    }
  }

  /// Copy full working memory into global memory buffer todo - test
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_FullGlobalToWorking(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    auto & mem_model = hw.GetMemoryModel();
    auto & global_mem_buffer = mem_model.GetGlobalBuffer();
    for (auto & mem : global_mem_buffer) {
      mem_state.SetWorking(mem.first, mem.second);
    }
  }

  // - Inst_Fork
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Fork(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    const emp::vector<size_t> matches(hw.FindModuleMatch(inst.GetTag(0)));
    if (matches.size()) {
      auto spawned = hw.SpawnThreadWithID(matches[0]);
      if (spawned) {
        const size_t thread_id = spawned.value();
        // Hold up there cowboy! If the module was empty, the hardware will ignore the CallModule request.
        if (hw.GetThread(thread_id).GetExecState().GetCallStack().size()) {
          // Spawned valid thread.
          // Do whatever it is that the memory model says we should do on a function call.
          emp_assert(hw.GetCurThread().GetExecState().GetCallStack().size());
          auto & forker = hw.GetCurThread().GetExecState().GetTopCallState();
          emp_assert(hw.GetThread(thread_id).GetExecState().GetCallStack().size());
          auto & forkee = hw.GetThread(thread_id).GetExecState().GetTopCallState();
          hw.GetMemoryModel().OnModuleCall(forker.GetMemory(), forkee.GetMemory());
        }
      }
    }
  }

  // - Inst_Terminate
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Terminate(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    hw.GetCurThread().SetDead();
  }

  /// Non-default instruction: Terminal
  /// Number of arguments: 1
  /// Description: writes a genetically-encoded value into a register.
  template<typename HARDWARE_T, typename INSTRUCTION_T,
           typename MaxRatio=std::ratio<1>, typename MinRatio=std::ratio<0>>
  static void Inst_Terminal(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    constexpr double max = static_cast<double>(MaxRatio::num) / MaxRatio::den;
    constexpr double min = static_cast<double>(MinRatio::num) / MinRatio::den;

    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();

    const auto & tag = inst.GetTag(0);

    const double val = (
      tag.GetDouble() / tag.MaxDouble()
    ) * (max - min) - min;

    mem_state.SetWorking(inst.GetArg(0), val);
  }

  /// Non-default instruction: SetRegulator
  /// Number of arguments: 2
  /// Description: Sets the regulator of a tag in the matchbin.
  template<typename HARDWARE_T, typename INSTRUCTION_T, int MULTIPLIER=1>
  void Inst_SetRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    emp::vector<size_t> best_fun(hw.GetMatchBin().MatchRaw(inst.GetTag(0), 1));
    if (best_fun.size() == 0) { return; }
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    const double regulator_val = MULTIPLIER * mem_state.AccessWorking(inst.GetArg(0));
    // (+) values down regulate
    // (-) values up regulate
    hw.GetMatchBin().SetRegulator(best_fun[0], regulator_val);
  }


  /// Non-default instruction: SetOwnRegulator
  /// Number of arguments: 2
  /// Description: Sets the regulator of the currently executing function.
  template<typename HARDWARE_T, typename INSTRUCTION_T, int MULTIPLIER=1>
  static void Inst_SetOwnRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    auto & flow = call_state.GetTopFlow();
    const double regulator_val = MULTIPLIER * mem_state.AccessWorking(inst.GetArg(0));
    // (+) values down regulate
    // (-) values up regulate
    hw.GetMatchBin().SetRegulator(flow.GetMP(), regulator_val);
  }

  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_ClearRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    emp::vector<size_t> best_fun(hw.GetMatchBin().MatchRaw(inst.GetTag(0), 1));
    if (best_fun.size() == 0) { return; }
    hw.GetMatchBin().SetRegulator(best_fun[0], 0);
  }

  template<typename HARDWARE_T, typename INSTRUCTION_T>
  static void Inst_ClearOwnRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & flow = call_state.GetTopFlow();
    hw.GetMatchBin().SetRegulator(flow.GetMP(), 0);
  }

  /// Non-default instruction: AdjRegulator
  /// Number of arguments: 3
  template<typename HARDWARE_T, typename INSTRUCTION_T, int MULTIPLIER=1>
  static void Inst_AdjRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    // const State & state = hw.GetCurState();
    emp::vector<size_t> best_fun = hw.GetMatchBin().MatchRaw(inst.GetTag(0), 1);
    if (!best_fun.size()) return;
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    const double adj = MULTIPLIER * mem_state.AccessWorking(inst.GetArg(0));
    hw.GetMatchBin().AdjRegulator(best_fun[0], adj);
  }

  /// Non-default instruction: AdjOwnRegulator
  /// Number of arguments: 3
  template<typename HARDWARE_T, typename INSTRUCTION_T, int MULTIPLIER=1>
  static void Inst_AdjOwnRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    auto & flow = call_state.GetTopFlow();
    const double adj = MULTIPLIER * mem_state.AccessWorking(inst.GetArg(0));
    hw.GetMatchBin().AdjRegulator(flow.GetMP(), adj);
  }

  /// Non-default instruction: IncRegulator
  /// Number of arguments: 3
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  static void Inst_IncRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    emp::vector<size_t> best_fun = hw.GetMatchBin().MatchRaw(inst.GetTag(0), 1);
    if (!best_fun.size()) return;
    hw.GetMatchBin().AdjRegulator(best_fun[0], 1.0);
  }

  template<typename HARDWARE_T, typename INSTRUCTION_T>
  static void Inst_IncOwnRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & flow = call_state.GetTopFlow();
    hw.GetMatchBin().AdjRegulator(flow.GetMP(), 1.0);
  }

  template<typename HARDWARE_T, typename INSTRUCTION_T>
  static void Inst_DecRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    emp::vector<size_t> best_fun = hw.GetMatchBin().MatchRaw(inst.GetTag(0), 1);
    if (!best_fun.size()) return;
    hw.GetMatchBin().AdjRegulator(best_fun[0], -1.0);
  }

  template<typename HARDWARE_T, typename INSTRUCTION_T>
  static void Inst_DecOwnRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & flow = call_state.GetTopFlow();
    hw.GetMatchBin().AdjRegulator(flow.GetMP(), -1.0);
  }

  /// Non-default instruction: SenseRegulator
  /// Number of arguments: 1
  /// Description: senses the value of the regulator of another function.
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  static void Inst_SenseRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
    emp::vector<size_t> best_fun = hw.GetMatchBin().MatchRaw(inst.GetTag(0), 1);
    if (best_fun.size()) {
      auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
      auto & mem_state = call_state.GetMemory();
      mem_state.SetWorking(inst.GetArg(0), hw.GetMatchBin().ViewRegulator(best_fun[0]));
    }
  }

  /// Non-default instruction: SenseOwnRegulator
  /// Number of arguments: 1
  /// Description: senses the value of the regulator the current function.
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  static void Inst_SenseOwnRegulator(HARDWARE_T & hw, const INSTRUCTION_T & inst){
    auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
    auto & mem_state = call_state.GetMemory();
    auto & flow = call_state.GetTopFlow();
    mem_state.SetWorking(inst.GetArg(0), hw.GetMatchBin().ViewRegulator(flow.GetMP()));
  }

  // - Inst_Nop
  ///  - do nothing
  template<typename HARDWARE_T, typename INSTRUCTION_T>
  void Inst_Nop(HARDWARE_T & hw, const INSTRUCTION_T & inst) { ; }

}}

#endif