#pragma once

#include <tuple>
#include <utility>

#include "base/vector.h"
#include "Evolve/World.h"
#include "tools/Random.h"
#include "tools/hash_utils.h"
#include "tools/BitSet.h"

#include "Cardi.h"
#include "DishWorld.h"
#include "Config.h"
#include "FrameCell.h"
#include "Genome.h"
#include "LibraryEvent.h"
#include "LibraryInstruction.h"
#include "Manager.h"
#include "Mutator.h"


DishWorld::DishWorld(const Config &cfg_, size_t uid_offset/*=0*/)
: cfg(cfg_)
, mut(cfg_)
, inst_lib(LibraryInstruction::Make(cfg))
, event_lib(LibraryEvent::Make(cfg))
{
  SetPopStruct_Grid(cfg.GRID_W(), cfg.GRID_H());

  for(size_t i = 0; i < GetSize(); ++i) {
    local_rngs.push_back(
      emp::NewPtr<emp::Random>(emp::szudzik_hash(cfg.SEED(),i+uid_offset))
    );
    global_rngs.push_back(emp::NewPtr<emp::Random>(cfg.SEED()));
  }

  man = emp::NewPtr<Manager>(*this, local_rngs, global_rngs, cfg);

  for(size_t i = 0; i < GetSize(); ++i) {
    frames.push_back(emp::NewPtr<FrameCell>(
      *local_rngs[i],
      cfg,
      *man,
      i,
      inst_lib,
      event_lib
    ));
  }

  // OnOffspringReady([](){;}});
  // OnOrgDeath([](){;});

  OnPlacement([this](const size_t pos){
    man->Stockpile(pos).Reset();
    frames[pos]->Reset();
    if(local_rngs[pos]->GetDouble() < cfg.MUTATION_RATE()) {
      mut.ApplyMutations(GetOrg(pos).program,*local_rngs[pos]);
    }
    frames[pos]->SetProgram(GetOrg(pos).program);
    man->Inbox(pos).ClearInboxes();
  });

  OnUpdate([this](size_t upd){
    Pre();
    Mid();
    Post();
  });

  InitSystematics();

  for(size_t i = 0; i < GetSize(); ++i) {
    Genome g(
      *local_rngs[i],
      inst_lib,
      cfg);
    InjectAt(g, emp::WorldPosition(i));
    emp_assert(GetOrg(i).program.GetSize());
    man->Stockpile(i).InternalApplyHarvest(cfg.START_RESOURCE());
  }

}

DishWorld::~DishWorld() {
  for (auto &ptr : global_rngs) ptr.Delete();
  for (auto &ptr : local_rngs) ptr.Delete();
  for (auto &ptr : frames) ptr.Delete();
  man.Delete();
}

void DishWorld::InitSystematics() {

  if (cfg.SYSTEMATICS()) {

    AddSystematics(
      emp::NewPtr<emp::Systematics<Genome,Genome>>(
        [](Genome & o){return o;},
        true,
        true,
        false
      ),
      "systematics"
    );

    #ifndef EMSCRIPTEN
    auto& sf = SetupSystematicsFile(
      "systematics",
      "Systematics_" + std::to_string(cfg.SEED()) + ".csv",
      false
    );

    sf.SetTiming([this](const size_t upd){ return cfg.TimingFun(upd); });
    sf.AddVar(cfg.SEED(), "seed", "Random generator seed");
    sf.AddVar(STRINGIFY(GIT_VERSION_), "GIT_VERSION", "Software version");
    sf.PrintHeaderKeys();
    #endif

  }

}

void DishWorld::Pre() {
  for (size_t i = 0; i < GetSize(); ++i) {

    man->Priority(i).Reset();
    man->Apoptosis(i).Reset();

    for(size_t l = 0; l < cfg.NLEV(); ++l) {
      man->Wave(i,l).CalcNext(GetUpdate());
    }

    if (IsOccupied(i)) {
      man->Stockpile(i).ResolveExternalContributions();
      for(size_t l = 0; l < cfg.NLEV(); ++l) {
        man->Wave(i,l).HarvestResource();
      }
      frames[i]->QueueMessages(man->Inbox(i).GetInboxes());
    }

  }
}

void DishWorld::Mid() {
  for(size_t i = 0; i < GetSize(); ++i) {
    if (IsOccupied(i)) frames[i]->Process();
  }
}

void DishWorld::Post() {
  for(size_t i = 0; i < GetSize(); ++i) {

    const auto optional_tup = man->Priority(i).QueryPendingGenome();

    if(optional_tup) {
      SirePack sirepack;
      emp::Ptr<Genome> prog;
      std::tie(prog, sirepack) = *(optional_tup);

      AddOrgAt(prog, i, sirepack.par_pos);
      man->Channel(i).Inherit(
        sirepack.chanpack,
        sirepack.channel_gens,
        sirepack.replev
      );
      man->Family(i).Reset(GetUpdate());
      man->Family(i).SetParentPos(sirepack.par_pos);
      man->Family(i).SetPrevChan(sirepack.prev_chan);
      // check that parent hasn't been overwritten by a different birth
      if(man->Family(sirepack.par_pos).GetBirthUpdate() != GetUpdate()) {
        man->Family(sirepack.par_pos).AddChildPos(i);
        man->Channel(sirepack.par_pos).LogReprGen(sirepack.replev);
      }
    } else if (IsOccupied(i)) {
      // this block doesn't get run if a cell was just born here
      if (man->Stockpile(i).IsBankrupt()
          || man->Apoptosis(i).IsMarked()) {
        DoDeath(i);
        if (!man->Apoptosis(i).IsMarkedPartial()) {
          man->Channel(i).ClearIDs();
        }
      }
    }

    // dead cells with no channels have nullopt so no transmission will occur
    for(size_t l = 0; l < cfg.NLEV(); ++l) man->Wave(i,l).ResolveNext();

  }
}
