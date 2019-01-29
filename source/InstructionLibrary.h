#pragma once

#include "base/assert.h"
#include "tools/Random.h"
#include "tools/string_utils.h"
#include "hardware/InstLib.h"

#include "CellFrame.h"
#include "Config.h"

class InstructionLibrary {

public:

  using hardware_t = Config::hardware_t;
  using inst_lib_t = emp::InstLib<hardware_t>;
  using inst_t = inst_lib_t::inst_t;

  static void InitDefaultInstLib(inst_lib_t &il) {
    il.AddInst("Inc", hardware_t::Inst_Inc, 1, "Increment value in local memory Arg1");
    // il.AddInst("Dec", hardware_t::Inst_Dec, 1, "Decrement value in local memory Arg1");
    // il.AddInst("Not", hardware_t::Inst_Not, 1, "Logically toggle value in local memory Arg1");
    // il.AddInst("Add", hardware_t::Inst_Add, 3, "Local memory: Arg3 = Arg1 + Arg2");
    // il.AddInst("Sub", hardware_t::Inst_Sub, 3, "Local memory: Arg3 = Arg1 - Arg2");
    // il.AddInst("Mult", hardware_t::Inst_Mult, 3, "Local memory: Arg3 = Arg1 * Arg2");
    // il.AddInst("Div", hardware_t::Inst_Div, 3, "Local memory: Arg3 = Arg1 / Arg2");
    // il.AddInst("Mod", hardware_t::Inst_Mod, 3, "Local memory: Arg3 = Arg1 % Arg2");
    // il.AddInst("TestEqu", hardware_t::Inst_TestEqu, 3, "Local memory: Arg3 = (Arg1 == Arg2)");
    // il.AddInst("TestNEqu", hardware_t::Inst_TestNEqu, 3, "Local memory: Arg3 = (Arg1 != Arg2)");
    // il.AddInst("TestLess", hardware_t::Inst_TestLess, 3, "Local memory: Arg3 = (Arg1 < Arg2)");
    // il.AddInst("If", hardware_t::Inst_If, 1, "Local memory: If Arg1 != 0, proceed; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
    // il.AddInst("While", hardware_t::Inst_While, 1, "Local memory: If Arg1 != 0, loop; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
    // il.AddInst("Countdown", hardware_t::Inst_Countdown, 1, "Local memory: Countdown Arg1 to zero.", emp::ScopeType::BASIC, 0, {"block_def"});
    // il.AddInst("Close", hardware_t::Inst_Close, 0, "Close current block if there is a block to close.", emp::ScopeType::BASIC, 0, {"block_close"});
    // il.AddInst("Break", hardware_t::Inst_Break, 0, "Break out of current block.");
    // il.AddInst("Call", hardware_t::Inst_Call, 0, "Call function that best matches call affinity.", emp::ScopeType::BASIC, 0, {"affinity"});
    // il.AddInst("Return", hardware_t::Inst_Return, 0, "Return from current function if possible.");
    // il.AddInst("SetMem", hardware_t::Inst_SetMem, 2, "Local memory: Arg1 = numerical value of Arg2");
    // il.AddInst("CopyMem", hardware_t::Inst_CopyMem, 2, "Local memory: Arg1 = Arg2");
    // il.AddInst("SwapMem", hardware_t::Inst_SwapMem, 2, "Local memory: Swap values of Arg1 and Arg2.");
    // il.AddInst("Input", hardware_t::Inst_Input, 2, "Input memory Arg1 => Local memory Arg2.");
    // il.AddInst("Output", hardware_t::Inst_Output, 2, "Local memory Arg1 => Output memory Arg2.");
    // il.AddInst("Commit", hardware_t::Inst_Commit, 2, "Local memory Arg1 => Shared memory Arg2.");
    // il.AddInst("Pull", hardware_t::Inst_Pull, 2, "Shared memory Arg1 => Shared memory Arg2.");
    // il.AddInst("BroadcastMsg", hardware_t::Inst_BroadcastMsg, 0, "Broadcast output memory as message event.", emp::ScopeType::BASIC, 0, {"affinity"});
    // il.AddInst("SendMsg", hardware_t::Inst_SendMsg, 0, "Send output memory as message event.", emp::ScopeType::BASIC, 0, {"affinity"});
    // il.AddInst("Fork", hardware_t::Inst_Fork, 0, "Fork a new thread, using tag-based referencing to determine which function to call on the new thread.", emp::ScopeType::BASIC, 0, {"affinity"});
    // il.AddInst("Terminate", hardware_t::Inst_Terminate, 0, "Terminate current thread.");
    // il.AddInst("Nop", hardware_t::Inst_Nop, 0, "No operation.");
  }

  static void InitInternalActions(inst_lib_t &il, Config &cfg) {
    // * facings
    //   * set random facing
    //   * rotate facing CW/CCW
    //   * and then do it for all facings
    emp::Random temp_r;
    FacingSet temp(temp_r, cfg);

    // for(size_t i = 0; i < temp.GetNumFacings(); ++i) {
    //   il.AddInst(
    //     "Spin-Facing" /*+ emp::to_string(i)*/,
    //     [i](hardware_t & hw, const inst_t & inst){
    //       // hw.GetFacing().Spin(hw.GetRandom(), i);
    //     }
    //   );
    //   il.AddInst(
    //     "TurnCw-Facing" + emp::to_string(i),
    //     [](hardware_t & hw, const inst_t & inst){
    //       // hw.GetFacing().TurnCw(i);
    //     }
    //   );
    //   il.AddInst(
    //     "TurnCcw-Facing" + emp::to_string(i),
    //     [](hardware_t & hw, const inst_t & inst){
    //       // hw.GetFacing().TurnCcw(i);
    //     }
    //   );
    //   il.AddInst(
    //     "Unify-Facing" + emp::to_string(i),
    //     [](hardware_t & hw, const inst_t & inst){
    //       // hw.GetFacing().Unify(hw.GetRandom(), i);
    //     }
    //   );
    // }
    //
    //
    // //
    // il.AddInst(
    //   "DoApoptosis",
    //   [](hardware_t & hw, const inst_t & inst){
    //     emp_assert(false, "Not implemented."); // TODO
    //   },
    //   1,
    //   "TODO"
    // );
    //

  }

  static inst_lib_t & Make(Config &cfg) {

    static inst_lib_t il;

    if (il.GetSize() == 0) {

      il = *hardware_t::DefaultInstLib();

      // InitDefaultInstLib(il);
      //
      // InitInternalActions(il, cfg);

      // InitExernalActions(il, cfg);
      //
      // InitInternalSensors(il, cfg);
      //
      // InitExternalActions(il, cfg);

    }

    return il;

  }

    //   // * adjust endowment(s)
    //   //   * different granularities (measured in multiples of one reproduction)
    //   //   * different resource pots per level? or just try to reproduce
    //   for(size_t i = 0; i < NLEV + 1; ++i) {
    //     this->AddInst(
    //       "AdjustEndowment-Lev" + emp::to_string(i),
    //       [](hardware_t & hw, const inst_t & inst){
    //         state_t & state = hw.GetCurState();
    //         assert(false, "Not implemented."); // TODO
    //         // hw.TriggerEvent("SendMessage", inst.affinity, state.output_mem);
    //       }
    //     );
    //   }
    // }
    //
    // void InitExternalActions(Config &cfg) {
    //
    //   this->AddInst(
    //     "SendUnitResource",
    //     [](hardware_t & hw, const inst_t & inst){
    //       const FacingSet &f = hw.GetFacing();
    //
    //       double amt = hw.GetUID().GetFrame().RequestResourceAmt(config.REP_THRESH()) {
    //
    //         auto neigh = hw.GetTile().GetNeigh(f.Get(f.GetSendResource()));
    //
    //         if(*neigh) {
    //           **neigh.GetFrame().AcceptResource(amt);
    //         }
    //
    //       }
    //     }
    //   );
    //
    //   this->AddInst(
    //     "SendHalfResource",
    //     [](hardware_t & hw, const inst_t & inst){
    //
    //       const FacingSet &f = hw.GetFacing();
    //
    //       double amt = hw.GetTile().GetFrame().RequestResourceFrac(0.5) {
    //
    //         auto neigh = hw.GetTile().GetNeigh(f.Get(f.GetSendResource()));
    //
    //         if(*neigh) {
    //           **neigh.GetFrame().AcceptResource(amt);
    //         }
    //
    //       }
    //     }
    //   );
    //
    //   // general purpose messaging TODO
    //   // e.g., https://github.com/amlalejini/GECCO-2018-Evolving-Event-driven-Programs-with-SignalGP/blob/master/experiments/election/source/Experiment.h
    //   /* TODO make a way to filter messages to exclude messages from other ChannelIDs */
    //   this->AddInst(
    //     "SendMsg",
    //     [](hardware_t & hw, const inst_t & inst){
    //       state_t & state = hw.GetCurState();
    //       assert(false, "Not implemented."); // TODO
    //       // hw.TriggerEvent("SendMessage", inst.affinity, state.output_mem);
    //     },
    //     0,
    //     "Send output memory as message event to faced neighbor.", emp::emp::ScopeType::BASIC,
    //     0,
    //     {"affinity"}
    //   );
    //
    //   this->AddInst(
    //     "BroadcastMsg",
    //     [](hardware_t & hw, const inst_t & inst){
    //       state_t & state = hw.GetCurState();
    //       assert(false, "Not implemented."); // TODO
    //       // hw.TriggerEvent("BroadcastMessage", inst.affinity, state.output_mem);
    //     },
    //     0,
    //     "Broadcast output memory as message event.",
    //     emp::emp::ScopeType::BASIC,
    //     0,
    //     {"affinity"}
    //     );
    //
    //   for(size_t i = 0; i < NLEV+1; ++i) {
    //     this->AddInst(
    //       "TryReproduce-Lev" + emp::to_string(i),
    //       [](hardware_t & hw, const inst_t & inst){
    //         assert(false, "Not implemented."); // TODO
    //       }
    //     );
    //   }
    //
    // }
    //
    // void InitInternalSensors(Config &cfg) {
    //   this->AddInst(
    //     "QueryOwnStockpile" + emp::to_string(i),
    //     [](hardware_t & hw, const inst_t & inst){
    //       state_t & state = hw.GetCurState();
    //       assert(false, "Not implemented."); // TODO
    //       // state.SetLocal(inst.args[0], this->env_state==i);
    //     }
    //   );
    //
    //   for (int i = 0; i < NLEV + 1; ++i) {
    //     this->AddInst(
    //       "QueryOwnEndowment-Lev" + emp::to_string(i),
    //       [](hardware_t & hw, const inst_t & inst){
    //         state_t & state = hw.GetCurState();
    //         assert(false, "Not implemented."); // TODO
    //         // state.SetLocal(inst.args[0], this->env_state==i);
    //       },
    //       1,
    //       "Sense if current environment state is " + emp::to_string(i)"
    //     );
    //   }
    // }
    //
    // void InitExternalSensors(Config &cfg) {
    //   // sensor functions
    //   for (int i = 0; i < NLEV; ++i) {
    //     this->AddInst(
    //       "QueryFacingChannelKin-Lev" + emp::to_string(i),
    //       [](hardware_t & hw, const inst_t & inst){
    //         state_t & state = hw.GetCurState();
    //         assert(false, "Not implemented."); // TODO
    //         // state.SetLocal(inst.args[0], this->env_state==i);
    //       },
    //       1,
    //       "Sense if current environment state is " + emp::to_string(i)"
    //     );
    //   }
    //
    //   // get the raw channel of who is next door
    //   // potentially useful for aggregate count of distinct neighbors
    //   for (int i = 0; i < NLEV; ++i) {
    //     this->AddInst(
    //       "QueryFacingChannel-Lev" + emp::to_string(i),
    //       [](hardware_t & hw, const inst_t & inst){
    //         state_t & state = hw.GetCurState();
    //         assert(false, "Not implemented."); // TODO
    //         // state.SetLocal(inst.args[0], this->env_state==i);
    //       },
    //       1,
    //       "Sense if current environment state is " + emp::to_string(i)"
    //     );
    //   }
    //
    //   this->AddInst(
    //     "QueryFacingLive" + emp::to_string(i),
    //     [](hardware_t & hw, const inst_t & inst){
    //       state_t & state = hw.GetCurState();
    //       assert(false, "Not implemented."); // TODO
    //       // state.SetLocal(inst.args[0], this->env_state==i);
    //     },
    //     1,
    //     "Sense if current environment state is " + emp::to_string(i)"
    //   );
    //
    //   this->AddInst(
    //     "QueryFacingStockpile" + emp::to_string(i),
    //     [](hardware_t & hw, const inst_t & inst){
    //       state_t & state = hw.GetCurState();
    //       assert(false, "Not implemented."); // TODO
    //       // state.SetLocal(inst.args[0], this->env_state==i);
    //     },
    //     1,
    //     "Sense if current environment state is " + emp::to_string(i)"
    //   );
    //   /* TODO how to query own facing, e.g., to turn to respond to a message but then to reset your facing how you were */
    //
    // }

};
