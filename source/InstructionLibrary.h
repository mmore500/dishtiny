#pragma once

#include "base/assert.h"
#include "tools/Random.h"
#include "tools/string_utils.h"

#include "CellHardware.h"
#include "Config.h"

class InstructionLibrary {

public:

  using hardware_t = CellHardware;
  using inst_lib_t = hardware_t::inst_lib_t;
  using inst_t = hardware_t::inst_t;

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
    //   }
    // );
    //

  }

  static inst_lib_t & Make(Config &cfg) {

    static inst_lib_t il;

    if (il.GetSize() == 0) {

      il = *CellHardware::DefaultInstLib();

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
    //     "Send output memory as message event to faced neighbor.", emp::ScopeType::BASIC,
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
    //     emp::ScopeType::BASIC,
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
