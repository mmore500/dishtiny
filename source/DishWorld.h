#pragma once

#include <tuple>
#include <utility>

#include "base/vector.h"
#include "Evolve/World.h"
#include "tools/Random.h"
#include "tools/hash_utils.h"
#include "tools/BitSet.h"

#include "Cardi.h"
#include "CellFrame.h"
#include "Config.h"
#include "Genome.h"
#include "InstructionLibrary.h"
#include "EventLibrary.h"
#include "Manager.h"
#include "Mutator.h"

class WebInterface;

class DishWorld : public emp::World<Genome> {

private:

  friend class WebInterface;

  const Config &cfg;

  emp::vector<emp::Ptr<emp::Random>> global_rngs;
  emp::vector<emp::Ptr<emp::Random>> local_rngs;
  emp::vector<emp::Ptr<Config::hardware_t>> cpus;
  emp::vector<emp::Ptr<CellFrame>> frames;

  Mutator mut;
  emp::Ptr<Manager> man;

public:
  DishWorld(const Config &cfg_, size_t uid_offset=0)
  : cfg(cfg_), mut(cfg_) {
    SetPopStruct_Grid(cfg.GRID_W(), cfg.GRID_H());

    for(size_t i = 0; i < GetSize(); ++i) {
      local_rngs.push_back(
        emp::NewPtr<emp::Random>(emp::szudzik_hash(cfg.SEED(),i+uid_offset))
      );
      global_rngs.push_back(emp::NewPtr<emp::Random>(cfg.SEED()));
    }

    man = emp::NewPtr<Manager>(local_rngs, global_rngs, cfg);

    for(size_t i = 0; i < GetSize(); ++i) {
      cpus.push_back(emp::NewPtr<Config::hardware_t>(
        &InstructionLibrary::Make(
          [this](size_t pos){ return IsOccupied(pos); },
          cfg
        ),
        EventLibrary::Make(cfg),
        local_rngs[i]
      ));
      frames.push_back(emp::NewPtr<CellFrame>(
        local_rngs[i],
        cfg,
        *man,
        i
      ));
      cpus[i]->PushTrait(frames[i]);
    }

    OnOffspringReady(
      [this](Genome& g, size_t pos){
        return mut.ApplyMutations(g.program,*local_rngs[pos]);
      }
    );

    OnOrgDeath([this](size_t pos){ ; });

    OnPlacement([this](size_t pos){
      man->Stockpile(pos).Reset();
      cpus[pos]->ResetHardware();
      cpus[pos]->ResetProgram();
      emp_assert(!cpus[pos]->GetProgram().GetSize());
      cpus[pos]->GetTrait(0)->Reset();
      cpus[pos]->SetProgram(GetOrg(pos).program);
      emp_assert(cpus[pos]->GetProgram().GetSize());
      man->Inbox(pos).ClearInboxes();
      frames[pos]->SpinFacing();
    });

    OnUpdate([this](size_t upd){
      Pre();
      Mid();
      Post();
    });

    for(size_t i = 0; i < GetSize(); ++i) {
      Genome g(*local_rngs[i], InstructionLibrary::Make(
        [this](size_t pos){ return IsOccupied(pos); },
        cfg
      ), cfg);
      InjectAt(g, emp::WorldPosition(i));
      emp_assert(GetOrg(i).program.GetSize());
      man->Stockpile(i).InternalApplyHarvest(cfg.START_RESOURCE());
    }

  }

  ~DishWorld() {
    for (auto &ptr : global_rngs) ptr.Delete();
    for (auto &ptr : local_rngs) ptr.Delete();
    for (auto &ptr : cpus) { ptr.Delete(); }
    for (auto &ptr : frames) ptr.Delete();
    man.Delete();
  }

  void Pre() {
    for(size_t i = 0; i < GetSize(); ++i) {
      for(size_t l = 0; l < cfg.NLEV(); ++l) {
        man->Wave(i,l).CalcNext(GetUpdate());
        if (IsOccupied(i)) man->Wave(i,l).HarvestResource();
      }
      if (IsOccupied(i)) {
        man->Inbox(i).QueueMessages(
          *cpus[i],
          [this, i](size_t pos){
            return frames[i]->CheckInboxActivity(pos);
          }
        );
      }
    }
  }

  void Mid() {
    for(size_t i = 0; i < GetSize(); ++i) {
      if (IsOccupied(i)) {
        emp_assert(cpus[i]->GetProgram().GetSize());
        frames[i]->ReprPauseSetup();
        cpus[i]->SpawnCore(
          Config::hardware_t::affinity_t(),
          0.5,
          {},
          false
        );
        cpus[i]->Process(cfg.HARDWARE_STEPS());
      }
    }
  }

  void Post() {
    for(size_t i = 0; i < GetSize(); ++i) {
      auto optional_tup = man->Priority(i).QueryPendingGenome();
      if(optional_tup) {
        SirePack sirepack;
        emp::Ptr<Genome> prog;
        std::tie(prog, sirepack) = *(optional_tup);

        AddOrgAt(prog, i, sirepack.par_pos);
        man->Channel(i).Inherit(sirepack.chanpack,sirepack.channel_gens,sirepack.rep_lev);
        man->Family(i).Reset();
        man->Family(i).SetParentPos(sirepack.par_pos);
        man->Family(sirepack.par_pos).AddChildPos(i);
        man->Channel(sirepack.par_pos).LogReprGen(sirepack.rep_lev);
      }
      if (IsOccupied(i)) {
        man->Priority(i).Reset();
        man->Stockpile(i).ResolveExternalContributions();
        for(size_t l = 0; l < cfg.NLEV(); ++l) man->Wave(i,l).ResolveNext();
        if (man->Stockpile(i).IsBankrupt()
            || man->Apoptosis(i).IsMarked()) {
          DoDeath(i);
          if (!man->Apoptosis(i).IsMarkedPartial()) {
            man->Channel(i).ClearIDs();
          }
        }
        man->Apoptosis(i).Reset();
      }
    }
  }

};
