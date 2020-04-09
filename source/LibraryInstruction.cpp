#pragma once

#include <cmath>

#include "LibraryInstruction.h"

using hardware_t = LibraryInstruction::hardware_t;
using inst_lib_t = LibraryInstruction::inst_lib_t;
using inst_t = LibraryInstruction::inst_t;
using state_t = LibraryInstruction::state_t;

void LibraryInstruction::TRL(
  hardware_t & hw,
  const double dir_arg,
  const size_t lev,
  const bool inherit_regulators,
  const Config &cfg
){

  FrameHardware &fh = *hw.GetTrait();

  Manager &man = fh.Cell().Man();
  const size_t pos = fh.Cell().GetPos();

  if (
    cfg.REP_THRESH()
    > man.Stockpile(pos).QueryResource() - fh.CheckReproductionReserve()
  ) {
    return;
  } else {

    const size_t dir = fh.CalcDir(dir_arg);

    const SirePack sp = {
      pos,
      dir,
      Cardi::Opp[dir],
      lev,
      man.Channel(pos).GetGenCounter(),
      man.Channel(pos).GetExpirations(),
      man.Family(pos).GetCellGen(),
      *man.Channel(pos).GetIDs(),
      lev < cfg.NLEV() ? man.Family(pos).GetPrevChan() : *man.Channel(pos).GetID(cfg.NLEV() - 1),
      man.DW().GetOrgPtr(pos),
      inherit_regulators
        ? std::optional<decltype(fh.Cell().CopyMatchBins())>{
          fh.Cell().CopyMatchBins()
        } : std::nullopt,
      man.Stockpile(pos).QueryResource(),
      fh.Cell().GetDaughterImprint()
    };

    if (man.Priority(fh.Cell().GetNeigh(dir)).AddRequest(sp)) {
      man.Stockpile(pos).RequestResourceAmt(cfg.REP_THRESH());
    }

  }

}

void LibraryInstruction::InitDefault(inst_lib_t &il) {

  il.AddInst("Inc", Config::hardware_t::Inst_Inc, 1, "Increment value in local memory Arg1");
  il.AddInst("Dec", Config::hardware_t::Inst_Dec, 1, "Decrement value in local memory Arg1");
  il.AddInst("Not", Config::hardware_t::Inst_Not, 1, "Logically toggle value in local memory Arg1");
  il.AddInst("Add", Config::hardware_t::Inst_Add, 3, "Local memory: Arg3 = Arg1 + Arg2");
  il.AddInst("Sub", Config::hardware_t::Inst_Sub, 3, "Local memory: Arg3 = Arg1 - Arg2");
  il.AddInst("Mult", Config::hardware_t::Inst_Mult, 3, "Local memory: Arg3 = Arg1 * Arg2");
  il.AddInst("Div", Config::hardware_t::Inst_Div, 3, "Local memory: Arg3 = Arg1 / Arg2");
  il.AddInst("Mod", Config::hardware_t::Inst_Mod, 3, "Local memory: Arg3 = Arg1 % Arg2");
  il.AddInst("TestEqu", Config::hardware_t::Inst_TestEqu, 3, "Local memory: Arg3 = (Arg1 == Arg2)");
  il.AddInst("TestNEqu", Config::hardware_t::Inst_TestNEqu, 3, "Local memory: Arg3 = (Arg1 != Arg2)");
  il.AddInst("TestLess", Config::hardware_t::Inst_TestLess, 3, "Local memory: Arg3 = (Arg1 < Arg2)");
  il.AddInst("If", Config::hardware_t::Inst_If, 1, "Local memory: If Arg1 != 0, proceed; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("While", Config::hardware_t::Inst_While, 1, "Local memory: If Arg1 != 0, loop; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("Countdown", Config::hardware_t::Inst_Countdown, 1, "Local memory: Countdown Arg1 to zero.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("Close", Config::hardware_t::Inst_Close, 0, "Close current block if there is a block to close.", emp::ScopeType::BASIC, 0, {"block_close"});
  il.AddInst("Break", Config::hardware_t::Inst_Break, 0, "Break out of current block.");
  il.AddInst("Call", Config::hardware_t::Inst_Call, 0, "Call function that best matches call affinity.", emp::ScopeType::BASIC, 0, {"affinity"});
  il.AddInst("Return", Config::hardware_t::Inst_Return, 0, "Return from current function if possible.");
  il.AddInst("SetMem", Config::hardware_t::Inst_SetMem, 2, "Local memory: Arg1 = numerical value of Arg2");
  il.AddInst("CopyMem", Config::hardware_t::Inst_CopyMem, 2, "Local memory: Arg1 = Arg2");
  il.AddInst("SwapMem", Config::hardware_t::Inst_SwapMem, 2, "Local memory: Swap values of Arg1 and Arg2.");
  il.AddInst("Input", Config::hardware_t::Inst_Input, 2, "Input memory Arg1 => Local memory Arg2.");
  il.AddInst("Output", Config::hardware_t::Inst_Output, 2, "Local memory Arg1 => Output memory Arg2.");
  il.AddInst("Commit", Config::hardware_t::Inst_Commit, 2, "Local memory Arg1 => Shared memory Arg2.");
  il.AddInst("Pull", Config::hardware_t::Inst_Pull, 2, "Shared memory Arg1 => Shared memory Arg2.");
  il.AddInst("Fork", Config::hardware_t::Inst_Fork, 0, "Fork a new thread, using tag-based referencing to determine which function to call on the new thread.", emp::ScopeType::BASIC, 0, {"affinity"});
  il.AddInst("Terminate", Config::hardware_t::Inst_Terminate, 0, "Terminate current thread.");
  il.AddInst("Nop", Config::hardware_t::Inst_Nop, 0, "No operation.");
  il.AddInst("Rng", Config::hardware_t::Inst_RngDouble, 1, "Draw from onboard random number generator.");
  il.AddInst("SetRegulator", Config::hardware_t::Inst_SetRegulator, 1, "Sets the regulator of a tag in the matchbin.", emp::ScopeType::BASIC, 0, {"affinity"});
  il.AddInst("SetOwnRegulator", Config::hardware_t::Inst_SetOwnRegulator, 1, "Sets the regulator the currently executing function.");
  il.AddInst("AdjRegulator", Config::hardware_t::Inst_AdjRegulator, 2, "Adjusts the regulator of a tag in the matchbin towards a target.", emp::ScopeType::BASIC, 0, {"affinity"});
  il.AddInst("AdjOwnRegulator", Config::hardware_t::Inst_AdjOwnRegulator, 2, "Adjusts the regulator of a tag in the matchbin towards a target.");
  il.AddInst("SenseRegulator", Config::hardware_t::Inst_SenseRegulator, 1, "Senses the regulator of a tag in the matchbin.", emp::ScopeType::BASIC, 0, {"affinity"});

  il.AddInst(
    emp::to_string("Terminal", 1073741824),
    Config::hardware_t::Inst_Terminal<std::ratio<5>, std::ratio<-5>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("Terminal", 8),
    Config::hardware_t::Inst_Terminal<std::ratio<8>, std::ratio<-8>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("Terminal", 1),
    Config::hardware_t::Inst_Terminal<std::ratio<1>, std::ratio<-1>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("Terminal", 0.125),
    Config::hardware_t::Inst_Terminal<std::ratio<1,8>, std::ratio<-1,8>>,
    1,
    "Writes a genetically-determined value into a register."
  );
}

void LibraryInstruction::InitDefaultDup(inst_lib_t &il) {

  il.AddInst("DuplicateInc", Config::hardware_t::Inst_Inc, 1, "Increment value in local memory Arg1");
  il.AddInst("DuplicateDec", Config::hardware_t::Inst_Dec, 1, "Decrement value in local memory Arg1");
  il.AddInst("DuplicateNot", Config::hardware_t::Inst_Not, 1, "Logically toggle value in local memory Arg1");
  il.AddInst("DuplicateAdd", Config::hardware_t::Inst_Add, 3, "Local memory: Arg3 = Arg1 + Arg2");
  il.AddInst("DuplicateSub", Config::hardware_t::Inst_Sub, 3, "Local memory: Arg3 = Arg1 - Arg2");
  il.AddInst("DuplicateMult", Config::hardware_t::Inst_Mult, 3, "Local memory: Arg3 = Arg1 * Arg2");
  il.AddInst("DuplicateDiv", Config::hardware_t::Inst_Div, 3, "Local memory: Arg3 = Arg1 / Arg2");
  il.AddInst("DuplicateMod", Config::hardware_t::Inst_Mod, 3, "Local memory: Arg3 = Arg1 % Arg2");
  il.AddInst("DuplicateTestEqu", Config::hardware_t::Inst_TestEqu, 3, "Local memory: Arg3 = (Arg1 == Arg2)");
  il.AddInst("DuplicateTestNEqu", Config::hardware_t::Inst_TestNEqu, 3, "Local memory: Arg3 = (Arg1 != Arg2)");
  il.AddInst("DuplicateTestLess", Config::hardware_t::Inst_TestLess, 3, "Local memory: Arg3 = (Arg1 < Arg2)");
  il.AddInst("DuplicateIf", Config::hardware_t::Inst_If, 1, "Local memory: If Arg1 != 0, proceed; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("DuplicateWhile", Config::hardware_t::Inst_While, 1, "Local memory: If Arg1 != 0, loop; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("DuplicateCountdown", Config::hardware_t::Inst_Countdown, 1, "Local memory: Countdown Arg1 to zero.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("DuplicateClose", Config::hardware_t::Inst_Close, 0, "Close current block if there is a block to close.", emp::ScopeType::BASIC, 0, {"block_close"});
  il.AddInst("DuplicateBreak", Config::hardware_t::Inst_Break, 0, "Break out of current block.");
  il.AddInst("DuplicateCall", Config::hardware_t::Inst_Call, 0, "Call function that best matches call affinity.", emp::ScopeType::BASIC, 0, {"affinity"});
  il.AddInst("DuplicateReturn", Config::hardware_t::Inst_Return, 0, "Return from current function if possible.");
  il.AddInst("DuplicateSetMem", Config::hardware_t::Inst_SetMem, 2, "Local memory: Arg1 = numerical value of Arg2");
  il.AddInst("DuplicateCopyMem", Config::hardware_t::Inst_CopyMem, 2, "Local memory: Arg1 = Arg2");
  il.AddInst("DuplicateSwapMem", Config::hardware_t::Inst_SwapMem, 2, "Local memory: Swap values of Arg1 and Arg2.");
  il.AddInst("DuplicateInput", Config::hardware_t::Inst_Input, 2, "Input memory Arg1 => Local memory Arg2.");
  il.AddInst("DuplicateOutput", Config::hardware_t::Inst_Output, 2, "Local memory Arg1 => Output memory Arg2.");
  il.AddInst("DuplicateCommit", Config::hardware_t::Inst_Commit, 2, "Local memory Arg1 => Shared memory Arg2.");
  il.AddInst("DuplicatePull", Config::hardware_t::Inst_Pull, 2, "Shared memory Arg1 => Shared memory Arg2.");
  il.AddInst("DuplicateFork", Config::hardware_t::Inst_Fork, 0, "Fork a new thread, using tag-based referencing to determine which function to call on the new thread.", emp::ScopeType::BASIC, 0, {"affinity"});
  il.AddInst("DuplicateTerminate", Config::hardware_t::Inst_Terminate, 0, "Terminate current thread.");
  il.AddInst("DuplicateNop", Config::hardware_t::Inst_Nop, 0, "No operation.");
  il.AddInst("DuplicateRng", Config::hardware_t::Inst_RngDouble, 1, "Draw from onboard random number generator.");
  il.AddInst("DuplicateSetRegulator", Config::hardware_t::Inst_SetRegulator, 1, "Sets the regulator of a tag in the matchbin.", emp::ScopeType::BASIC, 0, {"affinity"});
  il.AddInst("DuplicateSetOwnRegulator", Config::hardware_t::Inst_SetOwnRegulator, 1, "Sets the regulator the currently executing function.");
  il.AddInst("DuplicateAdjRegulator", Config::hardware_t::Inst_AdjRegulator, 2, "Adjusts the regulator of a tag in the matchbin towards a target.", emp::ScopeType::BASIC, 0, {"affinity"});
  il.AddInst("DuplicateAdjOwnRegulator", Config::hardware_t::Inst_AdjOwnRegulator, 2, "Adjusts the regulator of a tag in the matchbin towards a target.");


  il.AddInst(
    emp::to_string("DuplicateTerminal", 1073741824),
    Config::hardware_t::Inst_Terminal<std::ratio<5>, std::ratio<-5>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("DuplicateTerminal", 8),
    Config::hardware_t::Inst_Terminal<std::ratio<8>, std::ratio<-8>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("DuplicateTerminal", 1),
    Config::hardware_t::Inst_Terminal<std::ratio<1>, std::ratio<-1>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("DuplicateTerminal", 0.125),
    Config::hardware_t::Inst_Terminal<std::ratio<1,8>, std::ratio<-1,8>>,
    1,
    "Writes a genetically-determined value into a register."
  );
}

void LibraryInstruction::InitInternalActions(inst_lib_t &il, const Config &cfg) {

  il.AddInst(
    "PutInternalMembraneBringer",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      fh.GetInternalMembrane().Put(
        inst.affinity,
        !state.GetLocal(inst.args[0]),
        2 + state.GetLocal(inst.args[1])
      );

    },
    2,
    "Place a tag that, by default, admits incoming messages it matches with.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "PutInternalMembraneBlocker",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      fh.GetInternalMembrane().Put(
        inst.affinity,
        state.GetLocal(inst.args[0]),
        2 + state.GetLocal(inst.args[1])
      );

    },
    2,
    "Place a tag that, by default, blocks incoming messages it matches with.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "PutExternalMembraneBringer",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      fh.GetExternalMembrane().Put(
        inst.affinity,
        !state.GetLocal(inst.args[0]),
        2 + state.GetLocal(inst.args[1])
      );

    },
    2,
    "Place a tag that, by default, admits incoming messages it matches with.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "PutExternalMembraneBlocker",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      fh.GetExternalMembrane().Put(
        inst.affinity,
        state.GetLocal(inst.args[0]),
        2 + state.GetLocal(inst.args[1])
      );

    },
    2,
    "Place a tag that, by default, blocks incoming messages it matches with.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "SendMsgInternal",
    [](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();
      const size_t dir = fh.CalcDir(
        inst.args[0] + state.GetLocal(inst.args[1])
      );
      fh.SetMsgDir(dir);
      hw.TriggerEvent("SendMsgInternal", inst.affinity, state.output_mem);
    },
    2,
    "Send a single message to a particular other CPU within the cell.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "SendMsgSpiker",
    [](hardware_t & hw, const inst_t & inst){
      const state_t & state = hw.GetCurState();
      hw.TriggerEvent("SendMsgSpiker", inst.affinity, state.output_mem);
    },
    0,
    "Send a single message to a particular other CPU within the cell.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "BcstMsgInternal",
    [](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      for(size_t dir = 0; dir < Cardi::NumDirs; ++dir) {
        if (dir == fh.GetFacing()) continue;
        fh.SetMsgDir(dir);
        hw.TriggerEvent("SendMsgInternal", inst.affinity, state.output_mem);
      }

      hw.TriggerEvent("SendMsgSpiker", inst.affinity, state.output_mem);

    },
    0,
    "Send a single message all CPUs within the cell, excluding self.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "SetStockpileReserve",
    [&cfg](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();

      const state_t & state = hw.GetCurState();
      const size_t dir = fh.CalcDir();
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double amt = std::max(
        cfg.REP_THRESH() + state.GetLocal(inst.args[1]),
        0.0
      );
      const double clean_amt = std::isfinite(amt) ? amt : 0.0;
      emp_assert(clean_amt >= 0.0);

      fh.Cell().GetFrameHardware(dir).SetStockpileReserve(clean_amt, dur);

    },
    2,
    "Set aside an amount of stockpile resource that is not eligible for sharing."
  );

  // just depend on decay
  // il.AddInst(
  //   "ClearStockpileReserve",

  il.AddInst(
    "SetReproductionReserve",
    [&cfg](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();

      const state_t & state = hw.GetCurState();
      const size_t dir = fh.CalcDir();
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double amt = std::max(
        cfg.REP_THRESH() + state.GetLocal(inst.args[1]),
        0.0
      );
      const double clean_amt = std::isfinite(amt) ? amt : 0.0;
      emp_assert(clean_amt >= 0.0);

      fh.Cell().GetFrameHardware(dir).SetReproductionReserve(clean_amt, dur);

    },
    2,
    "Set aside an amount of stockpile resource that is not eligible for using to reproduce."
  );

  // just depend on decay
  // il.AddInst(
  //   "ClearReproductionReserve",

  for(size_t replev = 0; replev < cfg.NLEV()+1; ++replev) {
    il.AddInst(
      "PauseRepr-Lev" + emp::to_string(replev),
      cfg.CHANNELS_VISIBLE() ?
      std::function<void(hardware_t &, const inst_t &)>(
        [replev](hardware_t & hw, const inst_t & inst){

          FrameHardware &fh = *hw.GetTrait();
          Manager &man = fh.Cell().Man();

          const state_t & state = hw.GetCurState();
          const size_t dir = fh.CalcDir();
          const size_t dur = 2 + state.GetLocal(inst.args[0]);

          man.Priority(fh.Cell().GetNeigh(dir)).PauseRepr(
            Cardi::Opp[dir],
            replev,
            dur
          );

        }
      ) : std::function<void(hardware_t &, const inst_t &)>(
        [](hardware_t & hw, const inst_t & inst){ ; }
      ),
      1,
      "Pause reproduction in a particular direction for a certain reproduction level for a certain duration."
    );
  }

}

void LibraryInstruction::InitExternalActions(inst_lib_t &il, const Config &cfg) {

  il.AddInst(
    "SendBigFracResource",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();
      Manager &man = fh.Cell().Man();

      const size_t pos = fh.Cell().GetPos();
      const size_t dir = fh.CalcDir();
      const size_t neigh = fh.Cell().GetNeigh(dir);
      const double arg_1 = (
        std::isfinite(state.GetLocal(inst.args[0]))
        ? state.GetLocal(inst.args[0])
        : 0.0
      );
      const double arg_2 = (
        std::isfinite(state.GetLocal(inst.args[1]))
        ? state.GetLocal(inst.args[1])
        : 0.0
      );

      const double in_resistance = man.Sharing(neigh).CheckInResistance(
        Cardi::Opp[dir]
      ); emp_assert(in_resistance >= 0); emp_assert(in_resistance <= 1.0);
      const double out_resistance =  man.Sharing(pos).CheckOutResistance(
        dir
      ); emp_assert(out_resistance >= 0); emp_assert(out_resistance <= 1.0);

      const double reserve = std::max(fh.CheckStockpileReserve()+arg_2, 0.0);
      const double reserve_frac = std::clamp(
        reserve / std::max(
          man.Stockpile(pos).QueryResource(),
          std::numeric_limits<double>::epsilon()
        ),
        0.0,
        1.0
      ); emp_assert(reserve_frac >= 0.0); emp_assert(reserve_frac <= 1.0);

      const double multiplier = (
        arg_1 == 0
        ? 1
        : (
          arg_1 < 0
          ? 1/-arg_1
          : arg_1
        )
      ); emp_assert(multiplier >= 0);

      const double req_frac = std::min(1.0, 0.5*multiplier);
      emp_assert(req_frac >= 0); emp_assert(req_frac <= 1.0);


      const double frac = std::clamp(
        (1.0 - in_resistance) * (1.0 - out_resistance) * req_frac
          - reserve_frac,
        0.0,
        1.0
      ); emp_assert(frac >= 0); emp_assert(frac <= 1.0);
      emp_assert(std::isfinite(frac));

      man.Sharing(pos).AddSharingRequest(
        dir,
        frac
      );

    },
    2,
    "Send a fraction of available stockpile resource to a neighbor."
  );

  il.AddInst(
    "SendSmallFracResource",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();
      Manager &man = fh.Cell().Man();

      const size_t pos = fh.Cell().GetPos();
      const size_t dir = fh.CalcDir();
      const size_t neigh = fh.Cell().GetNeigh(dir);
      const double arg_1 = (
        std::isfinite(state.GetLocal(inst.args[0]))
        ? state.GetLocal(inst.args[0])
        : 0.0
      );
      const double arg_2 = (
        std::isfinite(state.GetLocal(inst.args[1]))
        ? state.GetLocal(inst.args[1])
        : 0.0
      );

      const double in_resistance = man.Sharing(neigh).CheckInResistance(
        Cardi::Opp[dir]
      ); emp_assert(in_resistance >= 0.0); emp_assert(in_resistance <= 1.0);
      const double out_resistance =  man.Sharing(pos).CheckOutResistance(
        dir
      ); emp_assert(out_resistance >= 0.0); emp_assert(out_resistance <= 1.0);

      const double reserve = std::max(fh.CheckStockpileReserve()+arg_2, 0.0);
      const double reserve_frac = std::clamp(
        reserve / std::max(
          man.Stockpile(pos).QueryResource(),
          std::numeric_limits<double>::epsilon()
        ),
        0.0,
        1.0
      ); emp_assert(reserve_frac >= 0.0); emp_assert(reserve_frac <= 1.0);

      const double multiplier = (
        arg_1 == 0
        ? 1
        : (
          arg_1 < 0
          ? 1/-arg_1
          : arg_1
        )
      ); emp_assert(multiplier >= 0);

      const double req_frac = std::min(1.0, 0.01*multiplier);
      emp_assert(req_frac >= 0); emp_assert(req_frac <= 1.0);


      const double frac = std::clamp(
        (1.0 - in_resistance) * (1.0 - out_resistance) * req_frac
          - reserve_frac,
        0.0,
        1.0
      ); emp_assert(frac >= 0.0); emp_assert(frac <= 1.0);
      emp_assert(std::isfinite(frac));

      man.Sharing(pos).AddSharingRequest(
        dir,
        frac
      );

    },
    2,
    "Send a fraction of available stockpile resource to a neighbor."
  );

  il.AddInst(
    "SetAcceptSharingTrue",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();
      const size_t dir = fh.CalcDir(state.GetLocal(inst.args[2]));
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double set = std::abs(
        emp::Mod(state.GetLocal(inst.args[1]) + 2.0, 4.0) - 2.0
      ) / 2.0;
      const double clean_set = std::isfinite(set) ? set : 0.0;
      emp_assert(clean_set >= 0.0); emp_assert(clean_set <= 1.0);

      man.Sharing(pos).SetInResistance(dir, clean_set, dur);
    },
    3,
    "Mark self to accept resource contributions from neighbors."
  );

  il.AddInst(
    "SetAcceptSharingFalse",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();
      const size_t dir = fh.CalcDir(state.GetLocal(inst.args[2]));
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double set = std::abs(
        emp::Mod(state.GetLocal(inst.args[1]), 4.0) - 2.0
      ) / 2.0;
      const double clean_set = std::isfinite(set) ? set : 0.0;
      emp_assert(clean_set >= 0.0); emp_assert(clean_set <= 1.0);

      man.Sharing(pos).SetInResistance(dir, clean_set, dur);
    },
    3,
    "Mark self to not accept resource contributions from neighbors."
  );

  il.AddInst(
    "SetGiveSharingTrue",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();
      const size_t dir = fh.CalcDir(state.GetLocal(inst.args[2]));
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double set = std::abs(
        emp::Mod(state.GetLocal(inst.args[1]) + 2.0, 4.0) - 2.0
      ) / 2.0;
      const double clean_set = std::isfinite(set) ? set : 0.0;
      emp_assert(clean_set >= 0.0); emp_assert(clean_set <= 1.0);

      man.Sharing(pos).SetOutResistance(dir, clean_set, dur);
    },
    3,
    "Mark self to give resource contributions to neighbors."
  );

  il.AddInst(
    "SetGiveSharingFalse",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();
      const size_t dir = fh.CalcDir(state.GetLocal(inst.args[2]));
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double set = std::abs(
        emp::Mod(state.GetLocal(inst.args[1]), 4.0) - 2.0
      ) / 2.0;
      const double clean_set = std::isfinite(set) ? set : 0.0;
      emp_assert(clean_set >= 0.0); emp_assert(clean_set <= 1.0);

      man.Sharing(pos).SetOutResistance(dir, clean_set, dur);
    },
    3,
    "Mark self to not give resource contributions to neighbors."
  );

  il.AddInst(
    "SendMsgExternal",
    [](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();
      const size_t dir = fh.CalcDir(state.GetLocal(inst.args[0]));
      fh.SetMsgDir(dir);
      hw.TriggerEvent("SendMsgExternal", inst.affinity, state.output_mem);
    },
    1,
    "Send a single message to a neighbor.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "BcstMsgExternal",
    [](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      for(size_t dir = 0; dir < Cardi::NumDirs; ++dir) {
        fh.SetMsgDir(dir);
        hw.TriggerEvent("SendMsgExternal", inst.affinity, state.output_mem);
      }

    },
    0,
    "Send a message to all neighbors.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  for(size_t replev = 0; replev < cfg.NLEV()+1; ++replev) {
    il.AddInst(
      "TryReproduce-Lev" + emp::to_string(replev),
      [replev, &cfg](hardware_t & hw, const inst_t & inst){
        const state_t & state = hw.GetCurState();
        TRL(
          hw,
          0,
          replev,
          !state.GetLocal(inst.args[0]),
          cfg
        );
      },
      1,
      "Attempt reproduction at a particular reproduction level; if the cell age limit has been reached reproduction will be attempted at the next highest level."
    );
  }

  for(size_t lev = 0; lev < cfg.NLEV(); ++lev) {
    il.AddInst(
      "SetCellAgeBooster-Lev" + emp::to_string(lev),
      cfg.CHANNELS_VISIBLE() ?
      std::function<void(hardware_t &, const inst_t &)>(
        [lev](hardware_t & hw, const inst_t & inst){
          const state_t & state = hw.GetCurState();
          FrameHardware &fh = *hw.GetTrait();
          fh.Cell().Man().Channel(fh.Cell().GetPos()).SetCellAgeBooster(
            lev,
            2 + state.GetLocal(inst.args[2])
          );
        }
      ) : std::function<void(hardware_t &, const inst_t &)>(
        [](hardware_t & hw, const inst_t & inst){ ; }
      ),
      3,
      "Increase cell age at a particular reproduction level."
    );
  }

  il.AddInst(
    "DoApoptosisComplete",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();

      man.Apoptosis(pos).MarkComplete();

    },
    0,
    "Destroy the cell and its channel ID at the end of this update."
  );

  il.AddInst(
    "SetHeir",
    [](hardware_t &hw, const inst_t &inst){
      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();
      const size_t dir = fh.CalcDir();
      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();
      const size_t dur = 2 + state.GetLocal(inst.args[0]);

      man.Heir(pos).SetHeir(dir, dur);
    },
    1,
    "Designate a neighbor to share in recoverable resource if the cell dies."
  );
  // just rely on decay instead...
  // il.AddInst(
  //   "UnsetHeir",


}

void LibraryInstruction::InitInternalSensors(
  inst_lib_t &il,
  const Config &cfg
) {
  il.AddInst(
    "QueryOwnStockpile",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();
      const double amt = man.Stockpile(pos).QueryResource();

      state_t & state = hw.GetCurState();

      state.SetLocal(inst.args[0], amt);

    },
    1,
    "How much resource is in my stockpile?"
  );

  for(size_t lev = 0; lev < cfg.NLEV(); ++lev) {
    il.AddInst(
      "QueryChannelGen-Lev"+emp::to_string(lev),
      cfg.CHANNELS_VISIBLE() ?
      std::function<void(hardware_t &, const inst_t &)>(
        [lev](hardware_t & hw, const inst_t & inst){

          FrameHardware &fh = *hw.GetTrait();

          Manager &man = fh.Cell().Man();
          const size_t pos = fh.Cell().GetPos();
          const size_t gen = man.Channel(pos).GetGeneration(lev);

          state_t & state = hw.GetCurState();

          state.SetLocal(inst.args[0], gen);

        }
      ) : std::function<void(hardware_t &, const inst_t &)>(
        [](hardware_t & hw, const inst_t & inst){ ; }
      ),
      1,
      "What is my cell age at a particular level?"
    );
  }

}

void LibraryInstruction::InitExternalSensors(
  inst_lib_t &il,
  const Config &cfg
) {

  il.AddInst(
    "QueryIsLive",
    [](hardware_t & hw, const inst_t & inst){

      state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      state.SetLocal(
        inst.args[1],
        fh.IsLive(state.GetLocal(inst.args[0]))
      );

    },
    2,
    "Is the neighboring cell alive?"
  );

  for (size_t lev = 0; lev < cfg.NLEV(); ++lev) {
    il.AddInst(
      emp::to_string("QueryIsExpired", lev),
      cfg.CHANNELS_VISIBLE() ?
      std::function<void(hardware_t &, const inst_t &)>(
        [lev](hardware_t & hw, const inst_t & inst){

          state_t & state = hw.GetCurState();
          FrameHardware &fh = *hw.GetTrait();

          state.SetLocal(
            inst.args[1],
            fh.IsExpired(lev, state.GetLocal(inst.args[0]))
          );

        }
      ) : std::function<void(hardware_t &, const inst_t &)>(
        [](hardware_t & hw, const inst_t & inst){ ; }
      ),
      2,
      "Is the neighboring cell expired?"
    );
  }

  il.AddInst(
    "QueryIsCellChild",
    cfg.CHANNELS_VISIBLE() ?
    std::function<void(hardware_t &, const inst_t &)>(
      [](hardware_t & hw, const inst_t & inst){

        state_t & state = hw.GetCurState();
        FrameHardware &fh = *hw.GetTrait();

        state.SetLocal(
          inst.args[1],
          fh.IsCellChild(state.GetLocal(inst.args[0]))
        );

      }
    ) : std::function<void(hardware_t &, const inst_t &)>(
      [](hardware_t & hw, const inst_t & inst){ ; }
    ),
    2,
    "Is the neighbor cell my direct offspring?"
  );

  il.AddInst(
    "QueryIsCellParent",
    cfg.CHANNELS_VISIBLE() ?
    std::function<void(hardware_t &, const inst_t &)>(
      [](hardware_t & hw, const inst_t & inst){

        state_t & state = hw.GetCurState();
        FrameHardware &fh = *hw.GetTrait();

        state.SetLocal(
          inst.args[1],
          fh.IsCellParent(state.GetLocal(inst.args[0]))
        );
      }
    ) : std::function<void(hardware_t &, const inst_t &)>(
      [](hardware_t & hw, const inst_t & inst){ ; }
    ),
    2,
    "Is the neighbor cell my direct parent?"
  );

  for (size_t lev = 0; lev < cfg.NLEV(); ++lev) {
    il.AddInst(
      "QueryChannelMate-Lev" + emp::to_string(lev),
      cfg.CHANNELS_VISIBLE() ?
      std::function<void(hardware_t &, const inst_t &)>(
        [lev](hardware_t & hw, const inst_t & inst){

          state_t & state = hw.GetCurState();
          FrameHardware &fh = *hw.GetTrait();

          state.SetLocal(
            inst.args[1],
            fh.IsChannelMate(lev, state.GetLocal(inst.args[0]))
          );

        }
      ) : std::function<void(hardware_t &, const inst_t &)>(
        [](hardware_t & hw, const inst_t & inst){ ; }
      ),
      2,
      "Does the neighbor cell share my channel ID for a particular level?"
    );
  }

  il.AddInst(
    "QueryIsPropaguleChild",
    cfg.CHANNELS_VISIBLE() ?
    std::function<void(hardware_t &, const inst_t &)>(
      [](hardware_t & hw, const inst_t & inst){

        state_t & state = hw.GetCurState();
        FrameHardware &fh = *hw.GetTrait();

        state.SetLocal(
          inst.args[1],
          fh.IsPropaguleChild(state.GetLocal(inst.args[0]))
        );

      }
    ): std::function<void(hardware_t &, const inst_t &)>(
      [](hardware_t & hw, const inst_t & inst){}
    ),
    2,
    "Does the neighbor's high-level channel ID directly descend from my high-level channel ID?"
  );

  il.AddInst(
    "QueryIsPropaguleParent",
    cfg.CHANNELS_VISIBLE() ?
    std::function<void(hardware_t &, const inst_t &)>(
      [](hardware_t & hw, const inst_t & inst){
        state_t & state = hw.GetCurState();

        FrameHardware &fh = *hw.GetTrait();

        state.SetLocal(
          inst.args[1],
          fh.IsPropaguleParent(state.GetLocal(inst.args[0]))
        );

      }
    ) : std::function<void(hardware_t &, const inst_t &)>(
     [](hardware_t & hw, const inst_t & inst){}
    ),
    2,
    "Does my high-level channdl ID directly descend from the neighbor's high-level channel ID?"
  );

  il.AddInst(
    "QueryIsPoorerThan",
    [](hardware_t & hw, const inst_t & inst){
      state_t & state = hw.GetCurState();

      FrameHardware &fh = *hw.GetTrait();

      state.SetLocal(
        inst.args[1],
        fh.IsPoorerThan(state.GetLocal(inst.args[0]))
      );

    },
    2,
    "How much more resource is in my neighbor's stockpile than in mine?"
  );

  il.AddInst(
    "QueryIsOlderThan",
    [](hardware_t & hw, const inst_t & inst){
      state_t & state = hw.GetCurState();

      FrameHardware &fh = *hw.GetTrait();

      state.SetLocal(
        inst.args[1],
        fh.IsOlderThan(state.GetLocal(inst.args[0]))
      );

    },
    2,
    "Is my cell age greater than my neighbor's?"
  );

  // get the raw channel of who is next door
  // potentially useful for aggregate count of distinct neighbors
  for (size_t lev = 0; lev < cfg.NLEV(); ++lev) {
    il.AddInst(
      "QueryChannel-Lev" + emp::to_string(lev),
      cfg.CHANNELS_VISIBLE() ?
      std::function<void(hardware_t &, const inst_t &)>(
        [lev](hardware_t & hw, const inst_t & inst){
          state_t & state = hw.GetCurState();

          FrameHardware &fh = *hw.GetTrait();

          const size_t dir = fh.CalcDir(state.GetLocal(inst.args[0]));
          const size_t neigh = fh.Cell().GetNeigh(dir);

          if(fh.IsLive(state.GetLocal(inst.args[0]))) {
            Manager &man = fh.Cell().Man();
            const auto chanid = man.Channel(neigh).GetID(lev);
            if(chanid) state.SetLocal(inst.args[1], *chanid);
          } else {
            state.SetLocal(inst.args[1], false);
          }
        }
      ) : std::function<void(hardware_t &, const inst_t &)>(
        [](hardware_t & hw, const inst_t & inst){ ; }
      ),
      2,
      "What my neighbor's channel ID for a particular level?"
    );
  }

  il.AddInst(
    "QueryStockpile",
    [](hardware_t & hw, const inst_t & inst){
      state_t & state = hw.GetCurState();

      FrameHardware &fh = *hw.GetTrait();

      const size_t dir = fh.CalcDir(state.GetLocal(inst.args[0]));
      const size_t neigh = fh.Cell().GetNeigh(dir);

      if(fh.IsLive(state.GetLocal(inst.args[0]))) {
        Manager &man = fh.Cell().Man();
        const double amt = man.Stockpile(neigh).QueryResource();
        state.SetLocal(inst.args[1], amt);
      } else {
        state.SetLocal(inst.args[1], false);
      }
    },
    2,
    "How much resource does my neighbor have in its stockpile?"
  );

}

void LibraryInstruction::InitSpikerActions(inst_lib_t &il, const Config &cfg) {

il.AddInst(
  "BcstSpikeMsg",
  [](hardware_t & hw, const inst_t & inst){
    const state_t & state = hw.GetCurState();
    hw.TriggerEvent("BcstSpikeMsg", inst.affinity, state.output_mem);
  },
  0,
  "Broadcast a single message to connections.",
  emp::ScopeType::BASIC,
  0,
  {"affinity"}
);

il.AddInst(
  "BcstSpikeReverseMsg",
  [](hardware_t & hw, const inst_t & inst){
    const state_t & state = hw.GetCurState();
    hw.TriggerEvent("BcstSpikeReverseMsg", inst.affinity, state.output_mem);
  },
  0,
  "Broadcast a single message to connections.",
  emp::ScopeType::BASIC,
  0,
  {"affinity"}
);

}

void LibraryInstruction::InitDevoActions(inst_lib_t &il, const Config &cfg) {

  il.AddInst(
    "TryAddFledglingConnection",
    [](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      Manager &man = fh.Cell().Man();

      const double p = 0.5 * std::tanh(state.GetLocal(inst.args[0])) + 0.5;

      const size_t connection_cap = static_cast<size_t>(
        state.GetLocal(inst.args[1])
      ) + 2;

      const size_t pos = fh.Cell().GetPos();
      if (hw.GetRandom().P(p)) {
        man.Connection(pos).TryAddFledgling(connection_cap);
      }

    },
    2,
    "TODO"
  );

  il.AddInst(
    "PruneOutgoingConnection",
    [](hardware_t & hw, const inst_t & inst){
      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();

      const double p = 0.5 * std::tanh(-state.GetLocal(inst.args[0]) - 1) + 0.5;

      const size_t connection_floor = static_cast<size_t>(
        state.GetLocal(inst.args[1])
      );

      if (hw.GetRandom().P(p)) {
        man.Connection(pos).PruneOutgoingConnection(connection_floor);
      }

    },
    2,
    "TODO"
  );

  il.AddInst(
    "PruneIncomingConnection",
    [](hardware_t & hw, const inst_t & inst){
      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      const double p = 0.5 * std::tanh(-state.GetLocal(inst.args[0]) - 1) + 0.5;

      const size_t connection_floor = static_cast<size_t>(
        state.GetLocal(inst.args[1])
      );

      if (hw.GetRandom().P(p)) {
        fh.Cell().PruneIncomingConnection(connection_floor);
      }

    },
    2,
    "TODO"
  );

  il.AddInst(
    "AddDevoUpQuery",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();

      man.Connection(pos).AddQuery(
        inst.affinity,
        2 + state.GetLocal(inst.args[0]),
        std::tanh(state.GetLocal(inst.args[1]) + 1.0)
      );

    },
    2,
    "TODO",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "AddDevoDownQuery",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();

      man.Connection(pos).AddQuery(
        inst.affinity,
        2 + state.GetLocal(inst.args[0]),
        std::tanh(state.GetLocal(inst.args[1]) - 1.0)
      );

    },
    2,
    "TODO",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "PutDevoMembraneBringer",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      auto & spiker = fh.Cell().GetSpiker();

      spiker.GetExternalMembrane().Put(
        inst.affinity,
        !state.GetLocal(inst.args[0]),
        2 + state.GetLocal(inst.args[1])
      );

    },
    2,
    "Place a tag that, by default, attracts queries it matches with.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "PutDevoMembraneBlocker",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      auto & spiker = fh.Cell().GetSpiker();

      spiker.GetExternalMembrane().Put(
        inst.affinity,
        state.GetLocal(inst.args[0]),
        2 + state.GetLocal(inst.args[1])
      );

    },
    2,
    "Place a tag that, by default, repels queries it matches with.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

}

const inst_lib_t& LibraryInstruction::Make(const Config &cfg) {

  static inst_lib_t il;

  if (!il.GetSize()) {

    InitDefault(il);

    InitDefaultDup(il);

    InitInternalActions(il, cfg);

    InitInternalSensors(il, cfg);

    InitExternalActions(il, cfg);

    InitExternalSensors(il, cfg);

    InitSpikerActions(il, cfg);

    InitDevoActions(il, cfg);

    il.AddInst(
      "MarkDaughterImprint",
      [](hardware_t & hw, const inst_t & inst){

        const state_t & state = hw.GetCurState();
        FrameHardware &fh = *hw.GetTrait();
        FrameCell &cell = fh.Cell();

        cell.SetDaughterImprint(
          state.GetLocal(inst.args[0])
          ? std::nullopt
          : std::optional<decltype(inst.affinity)>{inst.affinity}
        );

      },
      1,
      "Mark or remove an imprint.",
      emp::ScopeType::BASIC,
      0,
      {"affinity"}
    );

    std::cout << "Instruction Library Size: " << il.GetSize() << std::endl;
    // save instruction library
    std::ofstream inst_lib_stream(
      emp::keyname::pack({
        {"title", "instruction_library"},
        {"treat", cfg.TREATMENT_DESCRIPTOR()},
        {"ext", ".csv"}
      })
    );
    il.PrintManifest(inst_lib_stream);
  }

  emp_assert(il.GetSize());

  return il;

}
