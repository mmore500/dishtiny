#pragma once

#include <algorithm>
#include <list>

#include "tools/Random.h"
#include "tools/random_utils.h"

#include "Cardi.h"
#include "Config.h"
#include "FacingSet.h"
#include "GeometryHelper.h"

//forward declaration
class Manager;

class CellFrame {

private:

  FacingSet facing_set;
  Manager &man;
  emp::Random &local_rng;

  emp::vector<size_t> neighs;
  const size_t pos;

  // arranged by incoming direction
  emp::vector<bool> inbox_active;

public:

  CellFrame(
    emp::Ptr<emp::Random> local_rng_,
    Config &cfg,
    Manager &man_,
    size_t pos_
  ) : facing_set(*local_rng_, cfg)
    , man(man_)
    , local_rng(*local_rng_)
    , neighs(GeometryHelper(cfg).CalcLocalNeighs(pos_))
    , pos(pos_)
    , inbox_active()
  {
    Reset();
  }

  void Reset() {
    facing_set.Spin(facing_set.GetAllFacings());
    emp::vector<bool>(Cardi::Dir::NumDirs).swap(inbox_active);
  }

  const size_t GetPos() {
    return pos;
  }

  FacingSet& GetFacingSet() {
    return facing_set;
  }

  Manager& GetManager() {
    return man;
  }

  const size_t GetNeigh(size_t dir) {
    return neighs[dir];
  }

  void SetInboxActivity(size_t dir, bool state) {
    inbox_active[dir] = state;
  }

  bool CheckInboxActivity(size_t dir) {
    return inbox_active[dir];
  }

};
