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
  // current signal state
  GridStack<int>* signal;
  // next signal state
  GridStack<int>* sigbuff;

  const size_t NLEV;

public:
  const int ACTIVATED = -1;
  const int READY = 0;
  const int QUIESCENT_MAX;

  SignalManager(
    DishtinyConfig& dconfig,
    GridSpec& spec)
    : signal(new GridStack<int>(READY, spec, dconfig.NLEV()))
    , sigbuff(new GridStack<int>(READY, spec, dconfig.NLEV()))
    , NLEV(dconfig.NLEV())
    , QUIESCENT_MAX(dconfig.QUIESCENT_MAX())
    { ; }

  /*
   * Accessor function.
   */
  inline size_t GetDepth() const {
    return NLEV;
  }

  /*
   * Set cell state to ACTIVATED if cell occupied and in READY state.
   */
  inline void TryTakeTap(size_t lev, int x, int y, emp::World<Organism>& w) {
    if (signal->Get(lev, x, y) == READY && w.IsOccupied(signal->GetID(x, y))) {
      TakeTap(lev, x, y);
    }
  }

  /*
   * Run activation-quiescence wave simulation forward one step.
   */
  inline void Propagate(ChannelManager& cm, emp::World<Organism>& w) {

    for (size_t lev = 0; lev < NLEV; ++lev) {
      PropagateLev(lev, cm, w);
    }

    // swap buffer in
    GridStack<int>* temp = sigbuff;
    sigbuff = signal;
    signal = temp;

  }

  /*
   * Accessor function.
   */
  inline bool IsActivated(size_t lev, size_t cell) const {
    return signal->Get(lev, cell) == ACTIVATED;
  }

private:
  /*
   * Set cell state to ACTIVATED.
   */
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
          TryTakeWave(lev, x, y+1, sendch, cm, w);
          TryTakeWave(lev, x, y-1, sendch, cm, w);
          TryTakeWave(lev, x+1, y, sendch, cm, w);
          TryTakeWave(lev, x-1, y, sendch, cm, w);
        }

        (*sigbuff)(lev,x,y) = NextState(state);
      }
    }

  }

  /*
  * Set cell state to ACTIVATED if match with cell channel, cell occupied, and
  * cell in READY state (for taps that come from other activated cells).
  */
  inline void TryTakeWave(
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

  /*
   * Calculate next cell state based on current cell state.
   * From ACTIVATED, cells are set to QUIESCENT_MAX then decrement back to
   * READY.
   */
  inline int NextState(int state) {

    int res;
    if (state == READY) {
      res = READY;
    } else if (state > 0) {
      // in quiescent state, decrement back towards READY
      res = state - 1;
    } else if (state == ACTIVATED) {
      res = QUIESCENT_MAX;
    } else {
      // control should never reach here -- would require cell in invalid state
      emp_assert(false, state);
      res = READY;
    }

    return res;
  }

};
