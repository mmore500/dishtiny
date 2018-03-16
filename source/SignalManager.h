#pragma once

#include "base/vector.h"
#include "base/assert.h"
#include "Evolve/World.h"

#include "ChannelManager.h"
#include "GridStack.h"
#include "DishtinyConfig.h"
#include "Organism.h"

class SignalManager {
private:
  GridStack<int>* signal;
  GridStack<int>* sigbuff;

public:
  static constexpr int ACTIVATED = -1;
  static constexpr int READY = 0;
  const int QUIESCENT_MAX;

  SignalManager(
    DishtinyConfig& dconfig,
    GridSpec& spec)
    : signal(new GridStack<int>(READY, spec, dconfig.NLEV()))
    , sigbuff(new GridStack<int>(READY, spec, dconfig.NLEV()))
    , QUIESCENT_MAX(dconfig.QUIESCENT_MAX())
    { ; }

  inline size_t GetDepth() const {
    return signal->GetDepth();
  }

  inline void TryTakeTap(size_t lev, int x, int y, emp::World<Organism>& w) {
    if (signal->Get(lev, x, y) == READY && w.IsOccupied(signal->GetID(x, y))) {
      TakeTap(lev, x, y);
    }
  }

  inline void Propagate(ChannelManager& cm, emp::World<Organism>& w) {

    for (size_t lev = 0; lev < signal->GetDepth(); ++lev) {
      PropagateLev(lev, cm, w);
    }

    // swap buffer in
    GridStack<int>* temp = sigbuff;
    sigbuff = signal;
    signal = temp;

  }

  inline bool IsActivated(size_t lev, size_t cell) const {
    return signal->Get(lev, cell) == ACTIVATED;
  }

private:
  inline void TakeTap(size_t lev, int x, int y) {
    (*signal)(lev, x, y) = ACTIVATED;
  }

  inline void PropagateLev(
    size_t lev,
    ChannelManager& cm,
    emp::World<Organism>& w
  ) {

    for (size_t x = 0; x < signal->GetWidth(); ++x) {
      for (size_t y = 0; y < signal->GetWidth(); ++y) {
        int state = signal->Get(lev,x,y);

        if (state == ACTIVATED) {
          int sendch = cm.GetChannel(lev,x,y);
          TryTakeTap(lev, x, y+1, sendch, cm, w);
          TryTakeTap(lev, x, y-1, sendch, cm, w);
          TryTakeTap(lev, x+1, y, sendch, cm, w);
          TryTakeTap(lev, x-1, y, sendch, cm, w);
        }

        (*sigbuff)(lev,x,y) = NextState(state);
      }
    }

  }


  // for taps that come from other activated cells
  inline void TryTakeTap(
    size_t lev,
    int x,
    int y,
    int sendch,
    ChannelManager& cm,
    emp::World<Organism>& w
    ) {
    if (cm.GetChannel(lev, x, y) == sendch) {
      // refer to function to process taps from events
      TryTakeTap(lev, x, y, w);
    }
  }

  inline int NextState(int state) {

    int res;
    if (state == READY) {
      res = READY;
    } else if (state > 0) {
      res = state - 1;
    } else if (state == ACTIVATED) {
      res = QUIESCENT_MAX;
    } else {
      emp_assert(false, state);
      res = READY;
    }

    return res;
  }

};
