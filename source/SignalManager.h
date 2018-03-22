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
  const int ACTIVATED;
  const int READY;
  const int QUIESCENT_MAX;

  const size_t NLEV;

  // current signal state
  GridStack<int>* signal;
  // next signal state
  GridStack<int>* sigbuff;

public:
  SignalManager(
    DishtinyConfig& dconfig,
    GridSpec& spec)
    : ACTIVATED(-1)
    , READY(0)
    , QUIESCENT_MAX(dconfig.QUIESCENT_MAX())
    , NLEV(dconfig.NLEV())
    , signal(new GridStack<int>(READY, spec, dconfig.NLEV()))
    , sigbuff(new GridStack<int>(READY, spec, dconfig.NLEV()))
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

  /*
   * Put Animators into a supplied vector.
   */
  void MakeAnimators(
      emp::vector<emp::Ptr<GridAnimator<int>>>& dest,
      emp::web::Canvas& c,
      emp::vector<std::function<void()>>& cbs_beforedraw,
      emp::vector<std::function<void()>>& cbs_afterdraw
    ) {
    for (size_t lev = 0; lev < signal->GetDepth(); ++lev) {
      emp::Ptr<int> min_sig = new int;
      *min_sig = 0;
      emp::Ptr<int> max_sig = new int;
      *max_sig = QUIESCENT_MAX;
      dest.push_back(new GridAnimator<int>(
          *signal,
          lev,
          min_sig,
          max_sig,
          c,
          cbs_beforedraw,
          cbs_afterdraw
        ));
      dest.push_back(new GridAnimator<int>(
          *sigbuff,
          lev,
          min_sig,
          max_sig,
          c,
          cbs_beforedraw,
          cbs_afterdraw
        ));
    }
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
      if (signal->Get(lev, x, y) == READY && w.IsOccupied(signal->GetID(x, y))) {
        (*sigbuff)(lev, x, y) = ACTIVATED;
      }
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
