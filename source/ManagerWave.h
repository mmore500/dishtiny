#pragma once

#include "base/Ptr.h"
#include "base/vector.h"

#include "Config.h"

class ManagerWave {

static const int active = 1;
static const int ready = 0;
static const int quiescent = -3;

private:

  emp::Random global_rng;

  int state;

  int next_state;

  emp::vector<emp::Ptr<const ManagerWave>> nieghs;

  ManagerChannel &mc;

  ManagerStockpile &ms;

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
      (global_x  % cfg.Lev(lev).EVENT_RADIUS*2 == seed_x)
      && (global_y % cfg.Lev(lev).EVENT_RADIUS*2 == seed_y)
    );

  }

public:
  ManagerWave(
    ManagerChannel &mc_,
    ManagerStockpile &ms_,
    size_t lev_,
    size_t global_x_,
    size_t global_y_,
    CustomConfig &c
  ) : global_rng(c.SEED())
  , state(ready)
  , mc(mc_)
  , ms(ms_)
  , lev(lev_)
  , global_x(global_x_)
  , global_y(global_y_)
  , config(config)
  { ; }

  void CalcNext() {

    //TODO add seed code here

    if (state > 0) {
      /* e.g., if active */
      next_state = quiescent;
    } else if (state < 0) {
      /* e.g., if recovering from quiescent */
      next_state = state + 1;
    } else /* e.g., if ready */
      /* TODO */
      next_state = Incoming();
    }
  }

  void ApplyNext() {
    state = next_state;
  }

  void Seed() {
    if (state == ready) state = active;
  }

};
