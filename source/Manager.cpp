#pragma once

#include <algorithm>

#include "tools/Random.h"

#include "Config.h"
#include "DishWorld.h"
#include "Manager.h"
#include "ManagerApoptosis.h"
#include "ManagerChannel.h"
#include "ManagerFamily.h"
#include "ManagerInbox.h"
#include "ManagerPriority.h"
#include "ManagerStockpile.h"
#include "ManagerWave.h"

Manager::Manager(
  DishWorld &dw_,
  emp::vector<emp::Ptr<emp::Random>> &local_rngs,
  emp::vector<emp::Ptr<emp::Random>> &global_rngs,
  const Config &cfg
) : dw(dw_), mws(GeometryHelper(cfg).GetLocalSize()) {

  const size_t size = GeometryHelper(cfg).GetLocalSize();

  for(size_t i = 0; i < size; ++i) {
    mas.push_back(
      emp::NewPtr<ManagerApoptosis>()
    );
    mcs.push_back(
      emp::NewPtr<ManagerChannel>(cfg, *local_rngs[i])
    );
    mfs.push_back(
      emp::NewPtr<ManagerFamily>()
    );
    mhs.push_back(
      emp::NewPtr<ManagerHeir>()
    );
    mis.push_back(
      emp::NewPtr<ManagerInbox>(cfg)
    );
    mss.push_back(
      emp::NewPtr<ManagerStockpile>(cfg)
    );
  }

  for (size_t i = 0; i < size; ++i) {

    const emp::vector<size_t> neighs = GeometryHelper(cfg).CalcLocalNeighs(i);

    emp::vector<std::function<void()>> refunders;
    emp::vector<std::function<bool(size_t lev)>> expiredcheckers;
    for (size_t n : neighs) {
      refunders.push_back(mss[n]->MakeRepRefunder());
      expiredcheckers.push_back(mcs[n]->MakeExpChecker());
    }

    mps.push_back(
      emp::NewPtr<ManagerPriority>(
        cfg,
        *local_rngs[i],
        refunders,
        expiredcheckers
      )
    );

  }

  /* ManagerWaves part one */
  for(size_t i = 0; i < size; ++i) {
    for(size_t l = 0; l < cfg.NLEV(); ++l) {
      mws[i].push_back(
        emp::NewPtr<ManagerWave>(
          *mcs[i],
          *mss[i],
          l,
          i,
          *global_rngs[i],
          cfg
      ));
    }
  }

  /* ManagerWaves part two */
  for(size_t i = 0; i < size; ++i) {
    auto neigh_poses = GeometryHelper(cfg).CalcLocalNeighs(i);
    for(size_t l = 0; l < cfg.NLEV(); ++l) {
      emp::vector<emp::Ptr<ManagerWave>> res(neigh_poses.size());
      std::transform(
        neigh_poses.begin(),
        neigh_poses.end(),
        res.begin(),
        [this, l](size_t pos) { return mws[pos][l]; }
      );
      mws[i][l]->SetNeighs(res);
    }
  }

}

Manager::~Manager() {
  for (auto &ptr : mas) ptr.Delete();
  for (auto &ptr : mcs) ptr.Delete();
  for (auto &ptr : mfs) ptr.Delete();
  for (auto &ptr : mhs) ptr.Delete();
  for (auto &ptr : mis) ptr.Delete();
  for (auto &ptr : mps) ptr.Delete();
  for (auto &ptr : mss) ptr.Delete();
  for (auto &vec : mws) for (auto &ptr : vec) ptr.Delete();
}

ManagerApoptosis& Manager::Apoptosis(size_t pos) {
  return *mas[pos];
}

ManagerChannel& Manager::Channel(size_t pos) {
  return *mcs[pos];
}

ManagerFamily& Manager::Family(size_t pos) {
  return *mfs[pos];
}

ManagerHeir& Manager::Heir(size_t pos) {
  return *mhs[pos];
}

ManagerInbox& Manager::Inbox(size_t pos) {
  return *mis[pos];
}

ManagerPriority& Manager::Priority(size_t pos) {
  return *mps[pos];
}

ManagerStockpile& Manager::Stockpile(size_t pos) {
  return *mss[pos];
}

ManagerWave& Manager::Wave(size_t pos, size_t lev) {
  return *mws[pos][lev];
}

DishWorld& Manager::DW() { return dw; }
