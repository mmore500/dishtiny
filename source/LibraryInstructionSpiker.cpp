#pragma once

#include <cmath>

#include "LibraryInstructionSpiker.h"

void LibraryInstructionSpiker::InitDefault(inst_lib_t &il) {

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
  il.AddInst("SetRegulator", Config::hardware_t::Inst_SetRegulator, 1, "Sets the regulator of a tag in the matchbin.");
  il.AddInst("SetOwnRegulator", Config::hardware_t::Inst_SetOwnRegulator, 1, "Sets the regulator the currently executing function.");
  il.AddInst("AdjRegulator", Config::hardware_t::Inst_AdjRegulator, 2, "Adjusts the regulator of a tag in the matchbin towards a target.");
  il.AddInst("AdjOwnRegulator", Config::hardware_t::Inst_AdjOwnRegulator, 2, "Adjusts the regulator of the currently executing function towards a target.");
  il.AddInst("SenseRegulator", Config::hardware_t::Inst_SenseRegulator, 1, "Senses the regulator of a tag in the matchbin.");

  il.AddInst(
    emp::to_string("Terminal", 5),
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

void LibraryInstructionSpiker::InitDefaultDup(inst_lib_t &il) {

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
  il.AddInst("DuplicateSetRegulator", Config::hardware_t::Inst_SetRegulator, 1, "Sets the regulator of a tag in the matchbin.");
  il.AddInst("DuplicateSetOwnRegulator", Config::hardware_t::Inst_SetOwnRegulator, 1, "Sets the regulator the currently executing function.");
  il.AddInst("DuplicateAdjRegulator", Config::hardware_t::Inst_AdjRegulator, 2, "Adjusts the regulator of a tag in the matchbin towards a target.");
  il.AddInst("DuplicateAdjOwnRegulator", Config::hardware_t::Inst_AdjOwnRegulator, 2, "Adjusts the regulator of the currently executing function towards a target.");

  il.AddInst(
    emp::to_string("DuplicateTerminal", 5),
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

void LibraryInstructionSpiker::InitInternalActions(
  inst_lib_t &il,
  const Config &cfg
) {

  il.AddInst(
    "SendBigFracResource",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();
      Manager &man = fh.Cell().Man();

      const size_t pos = fh.Cell().GetPos();
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

      double in_resistance = 0.0;
      const auto & developed = man.Connection(pos).ViewDeveloped();
      for (const auto & [neigh, cell] : developed) {
        in_resistance += man.Sharing(neigh).CheckInResistance(
          Cardi::Dir::NumDirs
        );
      }
      if (developed.size()) in_resistance /= developed.size();
      emp_assert(in_resistance >= 0.0); emp_assert(in_resistance <= 1.0);

      const double out_resistance =  man.Sharing(pos).CheckOutResistance(
        Cardi::Dir::NumDirs
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
      ); emp_assert(multiplier >= 0.0);

      const double req_frac = std::min(1.0, 0.5*multiplier);
      emp_assert(req_frac >= 0.0); emp_assert(req_frac <= 1.0);

      const double frac = std::clamp(
        (1.0 - in_resistance) * (1.0 - out_resistance) * req_frac
          - reserve_frac,
        0.0,
        1.0
      ); emp_assert(frac >= 0.0); emp_assert(frac <= 1.0);
      emp_assert(std::isfinite(frac));

      man.Sharing(pos).AddSharingRequest(
        Cardi::Dir::NumDirs,
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

      double in_resistance = 0.0;
      const auto & developed = man.Connection(pos).ViewDeveloped();
      for (const auto & [neigh, cell] : developed) {
        in_resistance += man.Sharing(neigh).CheckInResistance(
          Cardi::Dir::NumDirs
        );
      }
      if (developed.size()) in_resistance /= developed.size();
      emp_assert(in_resistance >= 0.0); emp_assert(in_resistance <= 1.0);

      const double out_resistance =  man.Sharing(pos).CheckOutResistance(
        Cardi::Dir::NumDirs
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
      ); emp_assert(multiplier >= 0.0);

      const double req_frac = std::min(1.0, 0.01*multiplier);
      emp_assert(req_frac >= 0.0); emp_assert(req_frac <= 1.0);

      const double frac = std::clamp(
        (1.0 - in_resistance) * (1.0 - out_resistance) * req_frac
          - reserve_frac,
        0.0,
        1.0
      ); emp_assert(frac >= 0.0); emp_assert(frac <= 1.0);
      emp_assert(std::isfinite(frac));

      man.Sharing(pos).AddSharingRequest(
        Cardi::Dir::NumDirs,
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
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double set = std::abs(
        emp::Mod(state.GetLocal(inst.args[1]) + 2.0, 4.0) - 2.0
      ) / 2.0;
      const double clean_set = std::isfinite(set) ? set : 0.0;
      emp_assert(clean_set >= 0.0); emp_assert(clean_set <= 1.0);

      man.Sharing(pos).SetInResistance(Cardi::Dir::NumDirs, clean_set, dur);
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
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double set = std::abs(
        emp::Mod(state.GetLocal(inst.args[1]), 4.0) - 2.0
      ) / 2.0;
      const double clean_set = std::isfinite(set) ? set : 0.0;
      emp_assert(clean_set >= 0.0); emp_assert(clean_set <= 1.0);

      man.Sharing(pos).SetInResistance(Cardi::Dir::NumDirs, clean_set, dur);
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
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double set = std::abs(
        emp::Mod(state.GetLocal(inst.args[1]) + 2.0, 4.0) - 2.0
      ) / 2.0;
      const double clean_set = std::isfinite(set) ? set : 0.0;
      emp_assert(clean_set >= 0.0); emp_assert(clean_set <= 1.0);

      man.Sharing(pos).SetOutResistance(Cardi::Dir::NumDirs, clean_set, dur);
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
      const size_t dur = 2 + state.GetLocal(inst.args[0]);
      const double set = std::abs(
        emp::Mod(state.GetLocal(inst.args[1]), 4.0) - 2.0
      ) / 2.0;
      const double clean_set = std::isfinite(set) ? set : 0.0;
      emp_assert(clean_set >= 0.0); emp_assert(clean_set <= 1.0);

      man.Sharing(pos).SetOutResistance(Cardi::Dir::NumDirs, clean_set, dur);
    },
    3,
    "Mark self to not give resource contributions to neighbors."
  );

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
    "BcstMsgInternal",
    [](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      for(size_t dir = 0; dir < Cardi::NumDirs; ++dir) {
        fh.SetMsgDir(dir);
        hw.TriggerEvent("SendMsgInternal", inst.affinity, state.output_mem);
      }

    },
    0,
    "Send a single message all CPUs within the cell, excluding self.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

}

void LibraryInstructionSpiker::InitExternalActions(
  inst_lib_t &il,
  const Config &cfg
) {

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

}

void LibraryInstructionSpiker::InitInternalSensors(
  inst_lib_t &il,
  const Config &cfg
) {

  il.AddInst(
    "QueryIncomingConnections",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();

      state_t & state = hw.GetCurState();

      state.SetLocal(inst.args[0], fh.Cell().GetIncomingConectionCount());

    },
    1,
    "How many incoming connections are established?"
  );

  il.AddInst(
    "QueryOutgoingConnections",
    [](hardware_t & hw, const inst_t & inst){

      FrameHardware &fh = *hw.GetTrait();
      const size_t pos = fh.Cell().GetPos();
      Manager &man = fh.Cell().Man();

      state_t & state = hw.GetCurState();

      state.SetLocal(
        inst.args[0],
        man.Connection(pos).GetOutgoingConnectionCount()
      );
      state.SetLocal(
        inst.args[1],
        man.Connection(pos).CalcMeanConnectionLength()
      );

    },
    2,
    "How many outgoing connections are established? And what is their average length?"
  );

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

void LibraryInstructionSpiker::InitSpikerActions(
  inst_lib_t &il,
  const Config &cfg
) {

  il.AddInst(
    "SendSpikeMsg",
    [](hardware_t & hw, const inst_t & inst){
      const state_t & state = hw.GetCurState();
      hw.TriggerEvent("SendSpikeMsg", inst.affinity, state.output_mem);
    },
    0,
    "Send a single message to connections.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "SendSpikeReverseMsg",
    [](hardware_t & hw, const inst_t & inst){
      const state_t & state = hw.GetCurState();
      hw.TriggerEvent("SendSpikeReverseMsg", inst.affinity, state.output_mem);
    },
    0,
    "Send a single message to connections.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

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

void LibraryInstructionSpiker::InitDevoActions(
  inst_lib_t &il,
  const Config &cfg
) {

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

      fh.GetExternalMembrane().Put(
        inst.affinity,
        !state.GetLocal(inst.args[0]),
        2 + state.GetLocal(inst.args[1])
      );

    },
    2,
    "Place a tag that, by default, attracts incoming queries it matches with.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "PutDevoMembraneBlocker",
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
    "Place a tag that, by default, repels incoming queries it matches with.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "TryAddFledglingConnection",
    [](hardware_t & hw, const inst_t & inst){
      FrameHardware &fh = *hw.GetTrait();
      const state_t & state = hw.GetCurState();

      Manager &man = fh.Cell().Man();

      const double p = 0.5 * std::tanh(state.GetLocal(inst.args[0])) + 0.5;

      const size_t connection_cap = static_cast<size_t>(
        state.GetLocal(inst.args[1])
      ) + 1;

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

}

void LibraryInstructionSpiker::InitSpikerParams(
  inst_lib_t &il,
  const Config &cfg
) {

  il.AddInst(
    "SetConnectionAgingParam",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();

      // note this sets the param relative to default
      // so zero sets it to default
      man.Connection(pos).SetAgingParam(
        state.GetLocal(inst.args[0])
      );

    },
    1,
    "TODO"
  );

  il.AddInst(
    "SetConnectionExploitParam",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();

      // note this sets the param relative to default
      // so zero sets it to default
      man.Connection(pos).SetExploitParam(
        state.GetLocal(inst.args[0])
      );

    },
    1,
    "TODO"
  );

  il.AddInst(
    "SetConnectionDevelopmentParam",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();

      // note this sets the param relative to default
      // so zero sets it to default
      man.Connection(pos).SetDevelopmentParam(
        state.GetLocal(inst.args[0])
      );

    },
    1,
    "TODO"
  );

  il.AddInst(
    "SetConnectionSensingParam",
    [](hardware_t & hw, const inst_t & inst){

      const state_t & state = hw.GetCurState();
      FrameHardware &fh = *hw.GetTrait();

      Manager &man = fh.Cell().Man();
      const size_t pos = fh.Cell().GetPos();

      // note this sets the param relative to default
      // so zero sets it to default
      man.Connection(pos).SetSensingParam(
        state.GetLocal(inst.args[0])
      );

    },
    1,
    "TODO"
  );

}

const inst_lib_t& LibraryInstructionSpiker::Make(const Config &cfg) {

  static inst_lib_t il;

  if (!il.GetSize()) {

    InitDefault(il);

    InitDefaultDup(il);

    InitInternalActions(il, cfg);

    InitInternalSensors(il, cfg);

    InitExternalActions(il, cfg);

    InitSpikerActions(il, cfg);

    InitDevoActions(il, cfg);

    InitSpikerParams(il, cfg);

    // save instruction library
    std::ofstream inst_lib_stream(
      emp::keyname::pack({
        {"title", "instruction_library_spiker"},
        {"treat", cfg.TREATMENT_DESCRIPTOR()},
        {"ext", ".csv"}
      })
    );
    il.PrintManifest(inst_lib_stream);

    std::cout << "Instruction Library Size: " << il.GetSize() << std::endl;

  }

  emp_assert(il.GetSize());

  return il;

}
