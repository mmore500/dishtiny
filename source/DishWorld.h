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
  /*
   * Geometry helper function.
   */
  inline size_t GetX(size_t id) const { return id % width; }
  /*
   * Geometry helper function.
   */
  inline size_t GetY(size_t id) const { return id / width; }
  /*
   * Geometry helper function.
   */
  inline size_t GetID(int x, int y) const {
    return emp::Mod(y, height) * width + emp::Mod(x, width);
  }

  emp::Random global_rng;
  emp::vector<emp::Ptr<emp::Random>> rngs;
  emp::vector<emp::Ptr<CellHardware>> cpus;
  emp::vector<emp::Ptr<emp::vector<size_t>>> neighs;

  emp::vector<emp::Ptr<ManagerChannel>> mcs;
  emp::vector<emp::Ptr<ManagerPriority>> mps;
  emp::vector<emp::Ptr<ManagerStockpile>> mss;
  emp::vector<emp::vector<emp::Ptr<ManagerWave>>> mws;

  PriorityManager pm;
  Mutator mut;

public:
  DishWorld(DishtinyConfig &c, size_t uid_offset=0)
  : global_rng(c.SEED())
  {
    SetPopStruct_Grid(c.WIDTH(), c.HEIGHT(), false);

    for(size_t i = 0; i < GetSize(); ++i) {
      rngs.push_back(
        new emp::Random(emp::szudzik_hash(c.SEED(),i+uid_offset))
      );
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

    for(size_t i = 0; i < GetSize(); ++i) {
      neighs.push_back(new emp::vector<size_t>());
      for(size_t d = 0; d < Cardi::NumDirs; ++d) {
        neighs[i]->push_back(GetID(
            ((int)GetX(i)) + Cardi::Dx[d],
            ((int)GetY(i)) + Cardi::Dy[d]
          )]);
      }
    }

    for(size_t i = 0; i < GetSize(); ++i) {
      pms.push_back(new PriorityManager(*rngs[i]));
    }

    SetMutFun([this](Genome &g, emp::Random&r){
      // must use TILE rng, not global rng
      return mut.ApplyMutation(g.program, g.rng);
    });

    SetAutoMutate();

    OnUpdate([this](size_t upd){
      // reset pms
    });

    void SetupManagers() {

      for(size_t i = 0; i < GetSize(); ++i) {
        frames.push_back(
          new ManagerChannel(/*TODO*/)
        );
      }

      for(size_t i = 0; i < GetSize(); ++i) {
        frames_next.push_back(
          new ManagerPriority(/*TODO*/)
        );
      }

      for(size_t i = 0; i < GetSize(); ++i) {
        frames_next.push_back(
          new ManagerStockpile(/*TODO*/)
        );
      }

      for(size_t i = 0; i < GetSize(); ++i) {
        frames_next.push_back(
          new ManagerStockpile(mcs[i], )
        );
      }

      for(size_t i = 0; i < GetSize(); ++i) {
        frames_next.push_back(
          new ManagerWave(mcs[i], mss[i]);
        );
      }

    }

  }


};
