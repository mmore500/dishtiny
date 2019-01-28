#pragma once

#include <algorithm>

#include "tools/Random.h"

#include "Config.h"
#include "ManagerChannel.h"
#include "ManagerPriority.h"
#include "ManagerStockpile.h"
#include "ManagerWave.h"

class Manager {

private:
  emp::vector<emp::Ptr<ManagerChannel>> mcs;
  emp::vector<emp::Ptr<ManagerPriority>> mps;
  emp::vector<emp::Ptr<ManagerStockpile>> mss;
  emp::vector<emp::vector<emp::Ptr<ManagerWave>>> mws;


public:
  Manager(
    emp::vector<emp::Ptr<emp::Random>> &local_rngs,
    emp::vector<emp::Ptr<emp::Random>> &global_rngs,
    Config &cfg
  ) : {

    const size_t size = GeometryHelper(cfg).GetLocalSize();

    for(size_t i = 0; i < size; ++i) {
      mcs.push_back(
        new ManagerChannel(/*TODO*/)
      );
    }

    for(size_t i = 0; i < size; ++i) {
      mps.push_back(
        new ManagerPriority(/*TODO*/)
      );
    }

    for(size_t i = 0; i < size; ++i) {
      mss.push_back(
        new ManagerStockpile(/*TODO*/)
      );
    }

    for(size_t i = 0; i < size; ++i) {
      mws.push_back();

      for(size_t l = 0; l < cfg.NLEV(); ++l) {
        mws[i].push_back(
          new ManagerWave(
            mcs[i],
            mss[i],
            l,
            i,
            global_rngs[i],
            cfg
        ));
      }
    }

    for(size_t i = 0; i < size; ++i) {
      auto neigh_poses = GeometryHelper(cfg).CalcLocalNeighs(i);

      for(size_t l = 0; l < cfg.NLEV(); ++l) {
        mws[i][l].SetNeighs(std::transform(
          neigh_poses.begin(),
          neigh_poses.end(),
          [this](size_t pos) { return mws[pos][l]; }
        ));
      }
    }

  }

  ManagerChannel& Channel(size_t pos) {
    return *mcs[pos];
  }

  ManagerPriority& Priority(size_t pos) {
    return *mps[pos];
  }

  ManagerStockpile& Stockpile(size_t pos) {
    return *mss[pos];
  }

  ManagerWave& Wave(size_t pos, size_t lev) {
    return *mws[pos][lev];
  }

};
