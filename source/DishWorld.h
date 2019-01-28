#pragma once

#include <utility>

#include "base/vector.h"
#include "Evolve/World.h"
#include "tools/Random.h"

#include "Cardi.h"
#include "CellHardware.h"
#include "Config.h"
#include "Genome.h"
#include "Manager.h"
#include "Mutator.h"

class DishWorld : public emp::World<Genome> {

private:

  Config &cfg;

  emp::vector<emp::Ptr<emp::Random>> global_rngs;
  emp::vector<emp::Ptr<emp::Random>> local_rngs;
  emp::vector<emp::Ptr<CellHardware>> cpus;

  Mutator mut;
  emp::Ptr<Manager> man;

public:
  DishWorld(Config &cfg_, size_t uid_offset=0)
  : cfg(cfg_), mut(cfg_) {
    SetPopStruct_Grid(cfg.GRID_W(), cfg.GRID_H(), false);

    for(size_t i = 0; i < GetSize(); ++i) {
      local_rngs.push_back(
        new emp::Random(emp::szudzik_hash(cfg.SEED(),i+uid_offset))
      );
      global_rngs.push_back(new emp::Random(cfg.SEED()));
    }

    man = new Manager(local_rngs, global_rngs, cfg);

    for(size_t i = 0; i < GetSize(); ++i) {
      Genome g(*local_rngs[i], cfg);
      InjectAt(g, i);
    }

    for(size_t i = 0; i < GetSize(); ++i) {
      cpus.push_back(
        new CellHardware(
          local_rngs[i],
          cfg,
          *man,
          i
        )
      );
    }

    // TOOD will need to do this manually
    // SetMutFun([this](Genome &g, emp::Random&r){
    //   // must use TILE rng, not global rng
    //   return mut.ApplyMutations(g.program,);
    // });
    //
    // SetAutoMutate();

    OnUpdate([this](size_t upd){
      Pre();
      Mid();
      Post();
    });

  }

  void Pre() {
    for(size_t i = 0; i < GetSize(); ++i) {
      if (IsOccupied(i)) {
        for(size_t l = 0; l < cfg.NLEV(); ++l) {
          man->Wave(i,l).CalcNext();
          man->Wave(i,l).HarvestResource();
        }
      }
    }
  }

  void Mid() {
    for(size_t i = 0; i < GetSize(); ++i) {
      if (IsOccupied(i)) cpus[i]->Process(cfg.HARDWARE_STEPS());
      // need to put a program on the cpu
    }
  }

  void Post() {
    for(size_t i = 0; i < GetSize(); ++i) {
      if (IsOccupied(i)) {
        man->Priority(i).Reset();
        man->Stockpile(i).ResolveExternalContributions();
        for(size_t l = 0; l < cfg.NLEV(); ++l) man->Wave(i,l).ResolveNext();
      }
    }
  }

};
