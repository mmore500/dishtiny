#pragma once

#include <algorithm>
#include <list>

#include "tools/Random.h"
#include "tools/random_utils.h"

#include "Cardi.h"
#include "Config.h"
#include "GeometryHelper.h"

//forward declaration
class Manager;

class CellFrame {

private:

  size_t facing;
  Manager &man;
  emp::Random &local_rng;

  emp::vector<size_t> neighs;
  const size_t pos;

  // arranged by incoming direction
  emp::vector<bool> inbox_active;

  size_t msgdir;

  double stockpile_reserve;

  emp::Ptr<emp::vector<bool>> repr_pause_cur;
  emp::Ptr<emp::vector<bool>> repr_pause_nxt;

public:

  CellFrame(
    emp::Ptr<emp::Random> local_rng_,
    const Config &cfg,
    Manager &man_,
    const size_t pos_
  ) : facing(Cardi::Spin(*local_rng_))
    , man(man_)
    , local_rng(*local_rng_)
    , neighs(GeometryHelper(cfg).CalcLocalNeighs(pos_))
    , pos(pos_)
    , inbox_active()
    , stockpile_reserve(0)
    , repr_pause_cur(emp::NewPtr<emp::vector<bool>>(cfg.NLEV()+1))
    , repr_pause_nxt(emp::NewPtr<emp::vector<bool>>(cfg.NLEV()+1, true))
  {
    Reset();
  }

  ~CellFrame() {
    repr_pause_cur.Delete();
    repr_pause_nxt.Delete();
  }

  void Reset() {
    facing = Cardi::Spin(local_rng);
    emp::vector<bool>(Cardi::Dir::NumDirs).swap(inbox_active);
    stockpile_reserve = 0;
    std::fill(repr_pause_cur->begin(), repr_pause_cur->end(), false);
    std::fill(repr_pause_nxt->begin(), repr_pause_nxt->end(), true);
  }

  double CheckStockpileReserve() const { return stockpile_reserve; }

  void AdjustStockpileReserve(const double amt) {
    stockpile_reserve = std::max(0.0,stockpile_reserve+amt);
  }

  void ReprPauseSetup() {
    std::fill(repr_pause_cur->begin(), repr_pause_cur->end(), false);
    std::swap(repr_pause_cur, repr_pause_nxt);
  }

  void PauseRepr(const size_t lev) { (*repr_pause_nxt)[lev] = true; }

  bool IsReprPaused(const size_t lev) const {
    return repr_pause_cur->at(lev) || repr_pause_nxt->at(lev);
  }

  size_t GetPos() const {
    return pos;
  }

  size_t GetFacing() const {
    return facing;
  }

  void SetFacing(const size_t new_facing) {
    facing = new_facing;
  }

  void SpinFacing() {
    SetFacing(Cardi::Spin(local_rng));
  }

  size_t GetMsgDir() const {
    return msgdir;
  }

  void SetMsgDir(const size_t new_dir) {
    msgdir = new_dir;
  }

  Manager& GetManager() {
    return man;
  }

  size_t GetNeigh(const size_t dir) const {
    return neighs[dir];
  }

  void SetInboxActivity(const size_t dir, const bool state) {
    inbox_active[dir] = state;
  }

  bool CheckInboxActivity(const size_t dir) const {
    return inbox_active[dir];
  }

};
