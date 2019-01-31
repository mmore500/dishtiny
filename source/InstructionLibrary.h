#pragma once

#include "base/assert.h"
#include "tools/Random.h"
#include "tools/string_utils.h"
#include "hardware/InstLib.h"

#include "CellFrame.h"
#include "Config.h"
#include "GeometryHelper.h"
#include "Manager.h"

class InstructionLibrary {

public:

  using hardware_t = Config::hardware_t;
  using inst_lib_t = Config::inst_lib_t;
  using inst_t = inst_lib_t::inst_t;
  using state_t = hardware_t::State;

  static void InitInternalActions(inst_lib_t il, Config &cfg) {
    // * facings
    //   * set random facing
    //   * rotate facing CW/CCW
    //   * and then do it for all facings
    emp::Random temp_r;
    FacingSet temp(temp_r, cfg);

    for(size_t i = 0; i < temp.GetNumFacings(); ++i) {
      il.AddInst(
        "Spin-Facing" /*+ emp::to_string(i)*/,
        [i](hardware_t & hw, const inst_t & inst){
          CellFrame &fr = *hw.GetTrait(0);
          fr.GetFacingSet().Spin(i);
        }
      );
      il.AddInst(
        "TurnCw-Facing" + emp::to_string(i),
        [i](hardware_t & hw, const inst_t & inst){
          CellFrame &fr = *hw.GetTrait(0);
          fr.GetFacingSet().Cw(i);
        }
      );
      il.AddInst(
        "TurnCcw-Facing" + emp::to_string(i),
        [i](hardware_t & hw, const inst_t & inst){
          CellFrame &fr = *hw.GetTrait(0);
          fr.GetFacingSet().Ccw(i);
        }
      );
      il.AddInst(
        "Unify-Facing" + emp::to_string(i),
        [i](hardware_t & hw, const inst_t & inst){
          CellFrame &fr = *hw.GetTrait(0);
          fr.GetFacingSet().Unify(i);
        }
      );
    }

    // * adjust endowment(s)
    //   * different granularities (measured in multiples of one reproduction)
    for(size_t i = 0; i < cfg.NLEV() + 1; ++i) {
      il.AddInst(
        "IncreaseEndowment-Lev" + emp::to_string(i),
        [i, &cfg](hardware_t & hw, const inst_t & inst){
          CellFrame &fr = *hw.GetTrait(0);
          fr.AdjEndowment(cfg.REP_THRESH(), i);
        }
      );
      il.AddInst(
        "DecreaseEndowment-Lev" + emp::to_string(i),
        [i, &cfg](hardware_t & hw, const inst_t & inst){
          CellFrame &fr = *hw.GetTrait(0);
          fr.AdjEndowment(-cfg.REP_THRESH(), i);
        }
      );
    }

  }

  static void InitExternalActions(inst_lib_t &il, Config &cfg) {

    il.AddInst(
      "SendUnitResource",
      [&cfg](hardware_t & hw, const inst_t & inst){

        CellFrame &fr = *hw.GetTrait(0);

        Manager &man = fr.GetManager();
        size_t pos = fr.GetPos();

        double amt = man.Stockpile(pos).RequestResourceAmt(cfg.REP_THRESH());

        size_t neigh = fr.GetNeigh(fr.GetFacingSet().GetSendResource());

        man.Stockpile(neigh).ExternalContribute(amt);

      }
    );

    il.AddInst(
      "SendHalfResource",
      [](hardware_t & hw, const inst_t & inst){

        CellFrame &fr = *hw.GetTrait(0);

        Manager &man = fr.GetManager();
        size_t pos = fr.GetPos();

        double amt = man.Stockpile(pos).RequestResourceFrac(0.5);

        size_t neigh = fr.GetNeigh(fr.GetFacingSet().GetSendResource());

        man.Stockpile(neigh).ExternalContribute(amt);
      }
    );

    // general purpose messaging TODO
    // e.g., https://github.com/amlalejini/GECCO-2018-Evolving-Event-driven-Programs-with-SignalGP/blob/master/experiments/election/source/Experiment.h
    /* TODO make a way to filter messages to exclude messages from other ChannelIDs */
    // il.AddInst(
    //   "SendMsg",
    //   [](hardware_t & hw, const inst_t & inst){
    //     state_t & state = hw.GetCurState();
    //     emp_assert(false, "Not implemented."); // TODO
    //     // hw.TriggerEvent("SendMessage", inst.affinity, state.output_mem);
    //   },
    //   0,
    //   "Send output memory as message event to faced neighbor.", emp::ScopeType::BASIC,
    //   0,
    //   {"affinity"}
    // );
    //
    // il.AddInst(
    //   "BroadcastMsg",
    //   [](hardware_t & hw, const inst_t & inst){
    //     state_t & state = hw.GetCurState();
    //     emp_assert(false, "Not implemented."); // TODO
    //     // hw.TriggerEvent("BroadcastMessage", inst.affinity, state.output_mem);
    //   },
    //   0,
    //   "Broadcast output memory as message event.",
    //   emp::ScopeType::BASIC,
    //   0,
    //   {"affinity"}
    // );

    for(size_t i = 0; i < cfg.NLEV()+1; ++i) {
      il.AddInst(
        "TryReproduce-Lev" + emp::to_string(i),
        [i, &cfg](hardware_t & hw, const inst_t & inst){

          CellFrame &fr = *hw.GetTrait(0);

          Manager &man = fr.GetManager();
          size_t pos = fr.GetPos();

          double endow = fr.GetEndowment(i);
          double req = endow + cfg.REP_THRESH();

          if(req >= man.Stockpile(pos).QueryResource()) {

            man.Stockpile(pos).RequestResourceAmt(req);

            size_t dir = fr.GetFacingSet().GetReproduction(i);
            auto cp = man.Channel(pos).GetIDs();

            man.Priority(fr.GetNeigh(dir)).ProcessSire(
                hw.GetProgram(),
                cp,
                { pos,   /* size_t par_pos,*/
                  dir,   /* size_t dir, */
                  i,     /* size_t rep_lev, */
                  endow  /* double endowment */
                }
              );

          }

        }
      );
    }

    il.AddInst(
      "DoApoptosis",
      [](hardware_t & hw, const inst_t & inst){

        CellFrame &fr = *hw.GetTrait(0);

        Manager &man = fr.GetManager();
        size_t pos = fr.GetPos();

        man.Stockpile(pos).Apoptosis();

      },
      0,
      "TODO"
    );

  }

  static void InitInternalSensors(inst_lib_t &il, Config &cfg) {
    il.AddInst(
      "QueryOwnStockpile",
      [](hardware_t & hw, const inst_t & inst){

        CellFrame &fr = *hw.GetTrait(0);

        Manager &man = fr.GetManager();
        size_t pos = fr.GetPos();
        double amt = man.Stockpile(pos).QueryResource();

        state_t & state = hw.GetCurState();

        state.SetLocal(inst.args[0], amt);

      },
      1,
      "TODO"
    );

    for (size_t i = 0; i < cfg.NLEV() + 1; ++i) {
      il.AddInst(
        "QueryOwnEndowment-Lev" + emp::to_string(i),
        [i](hardware_t & hw, const inst_t & inst){

          CellFrame &fr = *hw.GetTrait(0);

          double amt = fr.GetEndowment(i);

          state_t & state = hw.GetCurState();

          state.SetLocal(inst.args[0], amt);

        },
        1,
       "TODO"
      );
    }

  }

  static void InitExternalSensors(
    inst_lib_t &il,
    std::function<bool(size_t)> is_live,
    Config &cfg
  ) {
    // sensor functions
    for (size_t i = 0; i < cfg.NLEV(); ++i) {
      il.AddInst(
        "QueryFacingChannelKin-Lev" + emp::to_string(i),
        [i, &is_live](hardware_t & hw, const inst_t & inst){
          state_t & state = hw.GetCurState();

          CellFrame &fr = *hw.GetTrait(0);

          size_t dir = fr.GetFacingSet().GetNeighborSensor();
          size_t neigh = fr.GetNeigh(dir);

          bool live = is_live(neigh);
          state.SetLocal(inst.args[0], live);

          if(live) {
            Manager &man = fr.GetManager();
            size_t pos = fr.GetPos();
            bool match = man.Channel(pos).CheckMatch(man.Channel(neigh), i);
            state.SetLocal(inst.args[1], match);
          }
        },
        2,
        "TODO"
      );
    }

    // get the raw channel of who is next door
    // potentially useful for aggregate count of distinct neighbors
    for (size_t i = 0; i < cfg.NLEV(); ++i) {
      il.AddInst(
        "QueryFacingChannel-Lev" + emp::to_string(i),
        [i, &is_live](hardware_t & hw, const inst_t & inst){
          state_t & state = hw.GetCurState();

          CellFrame &fr = *hw.GetTrait(0);

          size_t dir = fr.GetFacingSet().GetNeighborSensor();
          size_t neigh = fr.GetNeigh(dir);

          bool live = is_live(neigh);
          state.SetLocal(inst.args[0], live);

          if(live) {
            Manager &man = fr.GetManager();
            Config::chanid_t chanid = man.Channel(neigh).GetID(i);
            state.SetLocal(inst.args[1], chanid);
          }
        },
        2,
        "TODO"
      );
    }

    il.AddInst(
      "QueryFacingStockpile",
      [&is_live](hardware_t & hw, const inst_t & inst){
        state_t & state = hw.GetCurState();

        CellFrame &fr = *hw.GetTrait(0);

        size_t dir = fr.GetFacingSet().GetNeighborSensor();
        size_t neigh = fr.GetNeigh(dir);

        bool live = is_live(neigh);
        state.SetLocal(inst.args[0], live);

        if(live) {
          Manager &man = fr.GetManager();
          double amt = man.Stockpile(neigh).QueryResource();
          state.SetLocal(inst.args[1], amt);
        }
      },
      2,
      "TODO"
    );

    // /* TODO how to query own facing, e.g., to turn to respond to a message but then to reset your facing how you were */
    //
    // emp::Random temp_r;
    // FacingSet temp(temp_r, cfg);
    //
    // for(size_t i = 0; i < temp.GetNumFacings(); ++i) {
    //
    //
    // }

  }

  static inst_lib_t & Make(std::function<bool(size_t)> is_live, Config &cfg) {

    static inst_lib_t il;

    if (il.GetSize() == 0) {

      il = *hardware_t::DefaultInstLib();

      InitInternalActions(il, cfg);

      InitInternalSensors(il, cfg);

      InitExternalActions(il, cfg);

      InitExternalSensors(il, is_live, cfg);

    }

    emp_assert(il.GetSize());

    return il;

  }

};
