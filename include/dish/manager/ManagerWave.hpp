#pragma once
#ifndef DISH_MANAGER_MANAGERWAVE_HPP_INCLUDE
#define DISH_MANAGER_MANAGERWAVE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/source/base/Ptr.h"
#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/Empirical/source/tools/Random.h"

#include "../config/Config.hpp"
#include "../topology/GeometryHelper.hpp"

namespace dish {

class ManagerWave {

private:

  static const int active = 1;
  static const int ready = 0;
  const int quiescent;

  emp::Random &global_rng;

  int state;

  int next_state;

  double last_harvest;

  const ManagerChannel &mc;
  ManagerStockpile &ms;
  emp::Ptr<const emp::vector<emp::Ptr<ManagerWave>>> neighs;

  const size_t lev;
  const size_t global_x;
  const size_t global_y;

  const Config &cfg;

  size_t IncomingWave() const {

    int res = ready;
    for (const auto &n : *neighs) {
      if (mc.CheckMatch(n->mc, lev)) {
        res = std::max(res, n->state);
      }
    }

    /* if taking a signal from neighbor, increment it by one */
    /* only take a signal if it's less than signal radius */
    return res > ready && res < cfg.Lev(lev).SIGNAL_RADIUS() ? res + 1 : ready;

  }

  size_t IncomingSeed(const size_t update) const {

    // only put out seeds at intervals
    if (update % (cfg.Lev(lev).EVENT_RADIUS() + 1)) return ready;

    /* when this is called we know we're in the ready state */
    const size_t event_size = cfg.Lev(lev).EVENT_RADIUS()*2;

    const size_t seed_x = global_rng.GetUInt(0,event_size);
    const size_t seed_y = global_rng.GetUInt(0,event_size);

    return (
      (global_x % event_size == seed_x)
      && (global_y % event_size == seed_y)
      && (mc.IsExpired(0) <= cfg.EXP_GRACE_PERIOD())
    ) ? active : ready;

  }

public:
  ManagerWave(
    const ManagerChannel &mc_,
    ManagerStockpile &ms_,
    const size_t lev_,
    const size_t pos,
    emp::Random &global_rng_,
    const Config &cfg_
  ) : quiescent(
      - static_cast<int>(cfg_.WAVE_REPLICATES())
      * static_cast<int>(cfg_.Lev(lev_).SIGNAL_RADIUS())
    )
  , global_rng(global_rng_)
  , state(ready)
  , next_state(ready)
  , mc(mc_)
  , ms(ms_)
  , lev(lev_)
  , global_x(GeometryHelper(cfg_).GetGlobalX(pos))
  , global_y(GeometryHelper(cfg_).GetGlobalY(pos))
  , cfg(cfg_)
  { ; }

  ~ManagerWave() {
    if (neighs) neighs.Delete();
  }

  void SetNeighs(const emp::vector<emp::Ptr<ManagerWave>> n) {
    neighs = emp::NewPtr<emp::vector<emp::Ptr<ManagerWave>>>(n.cbegin(), n.cend());
  }

  /* this should only be called on live cells */
  void CalcNext(const size_t update) {

    // this must be called every update to keep global rngs in sync
    const size_t incoming_seed = IncomingSeed(update);

    if (state > 0) {
      /* e.g., if active */
      next_state = quiescent;
    } else if (state < 0) {
      /* e.g., if recovering from quiescent */
      next_state = state + 1;
    } else /* e.g., if ready */ {
      next_state = mc.GetIDs() ? std::max(IncomingWave(),incoming_seed) : ready;
    }

  }

  void HarvestResource() {

    last_harvest = 0.0;

    /* e.g., if active */
    if (state > 0) {
      double amt = cfg.Lev(lev).ACTIVATION_COST();

      if (state <= cfg.Lev(lev).EVENT_RADIUS()) {
        amt += cfg.Lev(lev).HARVEST_VALUE();
      }

      last_harvest = amt;

      ms.InternalApplyHarvest(lev, amt);
    }

  }

  /* for data collection, not for simulation */
  double GetLastHarvest() const {
    return last_harvest;
  }

  /* this should only be called on live cells */
  void ResolveNext() {
    state = next_state;
  }

  /* for when a cell dies */
  void Kill() {
    if (state > ready) state = ready;
  }

  /* accessor function */
  int GetState() const {
    return state;
  }

};

} // namespace dish

#endif // #ifndef DISH_MANAGER_MANAGERWAVE_HPP_INCLUDE
