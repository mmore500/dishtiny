#pragma once

#include <utility>

#include "base/vector.h"
#include "Evolve/World.h"
#include "tools/Random.h"

#include "Cardi.h"
#include "CellFrame.h"
#include "CellHardware.h"
#include "DishtinyConfig.h"
#include "Genome.h"
#include "Mutator.h"
#include "PriorityManager.h"

class DishWorld : public emp::World<Genome> {

private:

  friend class InstructionLib;

  emp::vector<emp::Ptr<emp::Random>> global_rngs;
  emp::vector<emp::Ptr<emp::Random>> local_rngs;
  emp::vector<emp::Ptr<CellHardware>> cpus;

  Mutator mut;
  Manager man;

public:
  DishWorld(Config &cfg, size_t uid_offset=0)
  : {
    SetPopStruct_Grid(cfg.WIDTH(), cfg.HEIGHT(), false);

    for(size_t i = 0; i < GetSize(); ++i) {
      local_rngs.push_back(
        new emp::Random(emp::szudzik_hash(cfg.SEED(),i+uid_offset))
      );
      global_rngs.push_back(new emp::Random(cfg.SEED()));
    }

    for(size_t i = 0; i < GetSize(); ++i) {
      Genome g;
      InjectAt(g(rngs[i]), i);
    }

    for(size_t i = 0; i < GetSize(); ++i) {
      cpus.push_back(
        new CellHardware(/*TODO*/)
      );
    }

    SetMutFun([this](Genome &g, emp::Random&r){
      // must use TILE rng, not global rng
      return mut.ApplyMutation(g.program, g.rng);
    });

    SetAutoMutate();

    OnUpdate([this](size_t upd){
      Pre();
      for(size_t i = 0; i < GetSize(); ++i) {
        if (IsOccupied(i)) cpus[i].Process(cfg.HARDWARE_STEPS());
      }
      Post();
    });

  }

  void Pre() {
    for(size_t i = 0; i < GetSize(); ++i) {
      if (IsOccupied(i)) {
        man.Wave(i).CalcNext();
        man.Wave(i).HarvestResource();
      }
    }
  }

  void Post() {
    for(size_t i = 0; i < GetSize(); ++i) {
      if (IsOccupied(i)) {
        man.Priority(i).Reset();
        man.Stockpile(i).ResolveExternalContributions();
        man.Wave(i).ResolveNext();
      }
    }

};
