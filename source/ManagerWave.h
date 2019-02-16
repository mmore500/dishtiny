#pragma once

#include <algorithm>

#include "base/Ptr.h"
#include "base/vector.h"
#include "tools/Random.h"

#include "Config.h"
#include "GeometryHelper.h"

class ManagerWave {

static const int active = 1;
static const int ready = 0;
static const int quiescent = -3;

private:

  emp::Random global_rng;

  int state;

  int next_state;


  ManagerChannel &mc;
  ManagerStockpile &ms;
  emp::Ptr<emp::vector<emp::Ptr<ManagerWave>>> neighs;

  size_t lev;
  size_t global_x;
  size_t global_y;

  Config &cfg;

  size_t IncomingWave() {

    int res = ready;
    for (auto n : *neighs) {
      if (mc.CheckMatch(n->mc, lev)) {
        res = std::max(res, n->state);
      }
    }

    /* if taking a signal from neighbor, increment it by one */
    /* only take a signal if it's less than signal radius */
    return res > ready && res < cfg.Lev(lev).SIGNAL_RADIUS() ? res + 1 : ready;

  }

  size_t IncomingSeed(size_t update) {

    // only put out seeds at intervals
    if (update % (cfg.Lev(lev).EVENT_RADIUS() + 1)) return ready;

    /* when this is called we know we're in the ready state */
    size_t event_size = cfg.Lev(lev).EVENT_RADIUS()*2;

    size_t seed_x = global_rng.GetUInt(0,event_size);
    size_t seed_y = global_rng.GetUInt(0,event_size);

    return (
      (global_x % event_size == seed_x) && (global_y % event_size == seed_y)
    ) ? active : ready;

  }

public:
  ManagerWave(
    ManagerChannel &mc_,
    ManagerStockpile &ms_,
    size_t lev_,
    size_t pos,
    emp::Random &global_rng_,
    Config &cfg_
  ) : global_rng(global_rng_)
  , state(ready)
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

  void SetNeighs(emp::vector<emp::Ptr<ManagerWave>> n) {
    neighs = emp::NewPtr<emp::vector<emp::Ptr<ManagerWave>>>(n.begin(), n.end());
  }

  /* this should only be called on live cells */
  void CalcNext(size_t update) {

    if (state > 0) {
      /* e.g., if active */
      next_state = quiescent;
    } else if (state < 0) {
      /* e.g., if recovering from quiescent */
      next_state = state + 1;
    } else /* e.g., if ready */ {
      // this must be called every update to keep global rngs in sync
      const size_t incoming_seed = IncomingSeed(update);
      next_state = mc.GetIDs() ? std::max(IncomingWave(),incoming_seed) : ready;
    }

  }

  void HarvestResource() {

    /* e.g., if active */
    if (state > 0) {
      double amt = cfg.Lev(lev).ACTIVATION_COST();

      if (state <= cfg.Lev(lev).EVENT_RADIUS()) {
        amt += cfg.Lev(lev).HARVEST_VALUE();
      }

      ms.InternalApplyHarvest(amt);
    }

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
  const int GetState() {
    return state;
  }

};
