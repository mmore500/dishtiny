#pragma once

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
  emp::Ptr<emp::vector<emp::Ptr<const ManagerWave>>> neighs;

  size_t lev;
  size_t global_x;
  size_t global_y;

  Config &cfg;

  size_t IncomingWave() {

    int res = ready;
    for (n : neighs) {
      if (mc.CheckMatch(n->mc, lev)) {
        res = std::max(res, n->state);
      }
    }

    /* if taking a signal from neighbor, increment it by one */
    return res > 0 ? res + 1 : res;

  }

  bool IncomingSeed() {

    /* when this is called we know we're in the ready state */
    size_t seed_x = global_rng.GetUInt(0,cfg.Lev(lev).EVENT_RADIUS*2);
    size_t seed_y = global_rng.GetUInt(0,cfg.Lev(lev).EVENT_RADIUS*2);

    return (
      (global_x  % (cfg.Lev(lev).EVENT_RADIUS*2) == seed_x)
      && (global_y % (cfg.Lev(lev).EVENT_RADIUS*2) == seed_y)
    );

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

  void SetNeighs(emp::vector<emp::Ptr<const ManagerWave>> neighs_) {
    neighs = new emp::vector<emp::Ptr<const ManagerWave>>();
    *neighs = neighs_;
  }

  /* this should only be called on live cells */
  void CalcNext() {

    if (state > 0) {
      /* e.g., if active */
      next_state = quiescent;
    } else if (state < 0) {
      /* e.g., if recovering from quiescent */
      next_state = state + 1;
    } else /* e.g., if ready */
      /* TODO incorporate IncomingSeed */
      next_state = Incoming();
    }
  }

  /* this should only be called on live cells */
  void ApplyNext() {
    /* TODO actually apply the resource */
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
