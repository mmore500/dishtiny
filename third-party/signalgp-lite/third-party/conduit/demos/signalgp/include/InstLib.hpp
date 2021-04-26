#pragma once

#include "Trait.hpp"
#include "typedef.hpp"

inst_lib_t& make_inst_lib() {

  static inst_lib_t inst_lib;

  // default instructions
  // inst_lib.AddInst(
  //   "Inc",
  //   hardware_t::Inst_Inc,
  //   1,
  //   "Increment value in local memory Arg1"
  // );
  // inst_lib.AddInst(
  //   "Dec",
  //    hardware_t::Inst_Dec,
  //   1,
  //   "Decrement value in local memory Arg1"
  // );
  // inst_lib.AddInst(
  //   "Not",
  //   hardware_t::Inst_Not,
  //   1,
  //   "Logically toggle value in local memory Arg1"
  // );
  // inst_lib.AddInst(
  //   "Add",
  //   hardware_t::Inst_Add,
  //   3,
  //   "Local memory: Arg3 = Arg1 + Arg2"
  // );
  // inst_lib.AddInst(
  //   "Sub",
  //   hardware_t::Inst_Sub,
  //   3,
  //   "Local memory: Arg3 = Arg1 - Arg2"
  // );
  // inst_lib.AddInst(
  //   "Mult",
  //   hardware_t::Inst_Mult,
  //   3,
  //   "Local memory: Arg3 = Arg1 * Arg2"
  // );
  // inst_lib.AddInst(
  //   "Div",
  //   hardware_t::Inst_Div,
  //   3,
  //   "Local memory: Arg3 = Arg1 / Arg2"
  // );
  // inst_lib.AddInst(
  //   "Mod",
  //   hardware_t::Inst_Mod,
  //   3,
  //   "Local memory: Arg3 = Arg1 % Arg2"
  // );
  // inst_lib.AddInst(
  //   "TestEqu",
  //   hardware_t::Inst_TestEqu,
  //   3,
  //   "Local memory: Arg3 = (Arg1 == Arg2)"
  // );
  // inst_lib.AddInst(
  //   "TestNEqu",
  //   hardware_t::Inst_TestNEqu,
  //   3,
  //   "Local memory: Arg3 = (Arg1 != Arg2)"
  // );
  // inst_lib.AddInst(
  //   "TestLess",
  //   hardware_t::Inst_TestLess,
  //   3,
  //   "Local memory: Arg3 = (Arg1 < Arg2)"
  // );
  // inst_lib.AddInst(
  //   "Call",
  //   hardware_t::Inst_Call,
  //   0,
  //   "Call function that best matches call affinity."
  // );
  // inst_lib.AddInst(
  //   "Return",
  //   hardware_t::Inst_Return,
  //   0,
  //   "Return from current function if possible."
  // );
  // inst_lib.AddInst(
  //   "SetMem",
  //   hardware_t::Inst_SetMem,
  //   2,
  //   "Local memory: Arg1 = numerical value of Arg2"
  // );
  // inst_lib.AddInst(
  //   "CopyMem",
  //   hardware_t::Inst_CopyMem,
  //   2,
  //   "Local memory: Arg1 = Arg2"
  // );
  // inst_lib.AddInst(
  //   "SwapMem",
  //   hardware_t::Inst_SwapMem,
  //   2,
  //   "Local memory: Swap values of Arg1 and Arg2."
  // );
  // inst_lib.AddInst(
  //   "Input",
  //   hardware_t::Inst_Input,
  //   2,
  //   "Input memory Arg1 => Local memory Arg2."
  // );
  // inst_lib.AddInst(
  //   "Output",
  //   hardware_t::Inst_Output,
  //   2,
  //   "Local memory Arg1 => Output memory Arg2."
  // );
  // inst_lib.AddInst(
  //   "Commit",
  //   hardware_t::Inst_Commit,
  //   2,
  //   "Local memory Arg1 => Shared memory Arg2."
  // );
  // inst_lib.AddInst(
  //   "Pull",
  //   hardware_t::Inst_Pull,
  //   2,
  //   "Shared memory Arg1 => Shared memory Arg2."
  // );
  // inst_lib.AddInst(
  //   "Nop",
  //   hardware_t::Inst_Nop,
  //   0,
  //   "No operation."
  // );
  // inst_lib.AddInst(
  //   "Fork",
  //   hardware_t::Inst_Fork,
  //   0,
  //   "Fork a new thread. Local memory contents of callee are loaded into forked thread's input memory."
  // );
  // inst_lib.AddInst(
  //   "Terminate",
  //   hardware_t::Inst_Terminate,
  //   0,
  //   "Kill current thread."
  // );
  //
  // // flow control
  // inst_lib.AddInst(
  //   "If",
  //   hardware_t::Inst_If,
  //   1,
  //   "Local memory: If Arg1 != 0, proceed; else, skip block.",
  //   emp::ScopeType::BASIC,
  //   0,
  //   {"block_def"}
  // );
  // inst_lib.AddInst(
  //   "While",
  //   hardware_t::Inst_While,
  //   1,
  //   "Local memory: If Arg1 != 0, loop; else, skip block.",
  //   emp::ScopeType::BASIC,
  //   0,
  //   {"block_def"}
  // );
  // inst_lib.AddInst(
  //   "Countdown",
  //   hardware_t::Inst_Countdown,
  //   1,
  //   "Local memory: Countdown Arg1 to zero.",
  //   emp::ScopeType::BASIC,
  //   0,
  //   {"block_def"}
  // );
  // inst_lib.AddInst(
  //   "Close",
  //   hardware_t::Inst_Close,
  //   0,
  //   "Close current block if there is a block to close.",
  //   emp::ScopeType::BASIC,
  //   0,
  //   {"block_close"}
  // );
  // inst_lib.AddInst(
  //   "Break",
  //   hardware_t::Inst_Break,
  //   0,
  //   "Break out of current block."
  // );

  // message-passing
  inst_lib.AddInst(
    "Broadcast",
    [](hardware_t & hw, const inst_t & inst) {
      // Trigger a Msg event using the hardware that executed this instruction
      // where the event's data is the output memory of the sender.
      hw.TriggerEvent("Msg", inst.affinity, hw.GetCurState().output_mem);
    },
    0,
    "Send message to other SignalGP agent."
  );

  return inst_lib;

};

const inst_lib_t& get_inst_lib() {
  static inst_lib_t& cache = make_inst_lib();
  return cache;
}
