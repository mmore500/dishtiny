#pragma once

#include <tuple>
#include <utility>
#include <limits>

#include "base/vector.h"
#include "Evolve/World.h"
#include "tools/Random.h"
#include "tools/hash_utils.h"
#include "tools/BitSet.h"
#include "tools/keyname_utils.h"
#include "tools/string_utils.h"

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

  emp::Random rand_init(cfg.SEED());
  for(size_t i = 0; i < GetSize(); ++i) {
    local_rngs.push_back(
      emp::NewPtr<emp::Random>(rand_init.GetInt(
        1,
        1000000000
      ))
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

  OnOrgDeath([this](const size_t pos){
    double amt = std::max(0.0,man->Stockpile(pos).QueryResource());
    if (man->Apoptosis(pos).IsMarked()) {
      amt += cfg.REP_THRESH() * cfg.APOP_RECOVERY_FRAC();
    }

    const size_t h_count = man->Heir(pos).HeirCount();
    if(h_count) {
      const auto neighs = GeometryHelper(cfg).CalcLocalNeighs(pos);
      for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
        if(man->Heir(pos).IsHeir(dir)) {
          man->Stockpile(neighs[dir]).InternalAcceptResource(amt/h_count);
        }
      }
    }
    man->Stockpile(pos).Reset();
  });

  OnPlacement([this](const size_t pos){
    man->Apoptosis(pos).Reset();
    man->Stockpile(pos).Reset();
    man->Heir(pos).Reset();
    man->Priority(pos).Reset();
    frames[pos]->Reset();
    frames[pos]->SetProgram(GetOrg(pos).GetProgram());
    man->Inbox(pos).ClearInboxes();
  });

  OnUpdate([this](size_t upd){ Step(); });

  InitSystematics();

  for(size_t i = 0; i < GetSize(); ++i) {
    Genome g(
      *local_rngs[i],
      inst_lib,
      cfg);
    InjectAt(g, emp::WorldPosition(i));
    emp_assert(GetOrg(i).GetProgram().GetSize());
    man->Stockpile(i).InternalAcceptResource(cfg.START_RESOURCE());
  }

}

DishWorld::~DishWorld() {
  Clear();
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
      emp::keyname::pack({
        {"title", "systematics"},
        {"treat", cfg.TREATMENT_DESCRIPTOR()},
        {"seed", emp::to_string(cfg.SEED())},
        {"_emp_hash", STRINGIFY(EMPIRICAL_HASH_)},
        {"_source_hash", STRINGIFY(DISHTINY_HASH_)},
        {"ext", ".csv"}
      }),
      false
    );

    sf.SetTiming([this](const size_t upd){ return cfg.TimingFun(upd); });
    sf.AddVar(cfg.SEED(), "seed", "Random generator seed");
    sf.PrintHeaderKeys();
    #endif

  }

}

void DishWorld::Step() {

  // resolve pending state from last update
  for (size_t i = 0; i < GetSize(); ++i) {

    // only process cell interactions every ENV_TRIG_FREQ updates
    if (GetUpdate() % cfg.ENV_TRIG_FREQ()) break;

    if (
      const auto opt_sirepack = man->Priority(i).QueryPendingGenome();
      opt_sirepack
    ) {

      if (
        (opt_sirepack->replev == cfg.NLEV()) &&
        (local_rngs[i]->GetDouble() < cfg.PROPAGULE_MUTATION_RATE())
      ) {
        opt_sirepack->genome->DoMutations(mut,*local_rngs[i]);
      } else if (local_rngs[i]->GetDouble() < cfg.MUTATION_RATE()) {
        opt_sirepack->genome->DoMutations(mut, *local_rngs[i]);
      }


      AddOrgAt(opt_sirepack->genome, i, opt_sirepack->par_pos);

      frames[i]->SetRegulators(opt_sirepack->matchbins);

      man->Channel(i).Inherit(
        opt_sirepack->chanpack,
        opt_sirepack->channel_gens,
        opt_sirepack->replev
      );
      man->Family(i).Reset(GetUpdate());
      man->Family(i).SetParentPos(opt_sirepack->par_pos);
      man->Family(i).SetPrevChan(opt_sirepack->prev_chan);
      man->Family(i).SetCellGen(opt_sirepack->cell_gen, opt_sirepack->replev);
      // check that parent hasn't been overwritten by a different birth
      if(man->Family(opt_sirepack->par_pos).GetBirthUpdate() != GetUpdate()) {
        man->Family(opt_sirepack->par_pos).AddChildPos(i);
        // parental generation bump disabled
        // man->Channel(opt_sirepack->par_pos).LogReprGen(opt_sirepack->replev);
      }

      man->Priority(i).ClearPendingGenome();

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
  }

  for (size_t i = 0; i < GetSize(); ++i) {
    for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
      // dead cells with no channels have nullopt so no transmission will occur
      for (size_t l = 0; l < cfg.NLEV(); ++l) man->Wave(r,i,l).ResolveNext();
    }
  }

  // clean up for next cell action step
  for (size_t i = 0; i < GetSize(); ++i) {

    if (GetUpdate()%cfg.ENV_TRIG_FREQ()==0) {
      man->Heir(i).DecayHeirs();
      man->Channel(i).DecayCellAgeBoosters();
      man->Priority(i).ResolveUpdate();
      man->Apoptosis(i).Reset();
      man->Stockpile(i).ResolveNextResistance();
    }

    for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
      for(size_t l = 0; l < cfg.NLEV(); ++l) {
        man->Wave(r,i,l).CalcNext(GetUpdate());
      }
    }

    if (IsOccupied(i)) {
      man->Stockpile(i).ResolveExternalContributions();
      man->Stockpile(i).ApplyBaseInflow();
      man->Stockpile(i).ApplyDecay();
      for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
        for(size_t l = 0; l < cfg.NLEV(); ++l) {
          man->Wave(r,i,l).HarvestResource();
        }
      }
      if (GetUpdate() % cfg.COMPUTE_FREQ() == 0) {
        frames[i]->QueueMessages(man->Inbox(i).GetInboxes());
      }

      if (
        cfg.GEN_INCR_FREQ()
        && man->Family(i).GetCellAge(GetUpdate())
        && man->Family(i).GetCellAge(GetUpdate()) % cfg.GEN_INCR_FREQ() == 0
      ) man->Channel(i).IncrGenCounts();
    }

  }

  // do cell action!
  for(size_t i = 0; i < GetSize(); ++i) {
    if (IsOccupied(i)) {
      man->Stockpile(i).CleanSharingDoers(GetUpdate());
      frames[i]->Process(GetUpdate());
      man->Stockpile(i).ProcessSharingDoers(GetUpdate());
    }
  }

  // prepare for the next update
  // do it here instead of at the top so that DataHelper can view
  if (0 == (GetUpdate()+1) % cfg.ENV_TRIG_FREQ()) {
    for (size_t i = 0; i < GetSize(); ++i) {
      // decide who gets to be parent & make a copy of that genome
      // so that it doesn't get overwritten
      man->Priority(i).ResolveSire(man->Stockpile(i).QueryResource());
    }
  }

}
