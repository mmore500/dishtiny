#pragma once

#include "tools/Random.h"

#include "Cardi.h"
#include "Config.h"
#include "FacingSet.h"

//forward declaration
class Manager;

class CellFrame {

private:

  emp::vector<double> endowments;
  FacingSet facing_set;
  Manager &man;
  emp::Random &local_rng;

  const size_t pos;

public:

  CellFrame(
    emp::Ptr<emp::Random> local_rng_,
    Config &cfg,
    Manager &man_,
    size_t pos_
  ) : endowments(cfg.NLEV()+1)
    , facing_set(*local_rng_, cfg)
    , man(man_)
    , local_rng(*local_rng_)
    , pos(pos_)
  {
    Reset();
  }

  void Reset() {
    /* TODO why does this make everything run so fast */
    std::fill(endowments.begin(), endowments.end(), 0);
    facing_set.Spin(facing_set.GetAllFacings());
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

};
