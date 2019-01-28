#pragma once

#include "hardware/EventDrivenGP.h"

#include "Cardi.h"
#include "Config.h"
#include "FacingSet.h"
#include "Manager.h"


class CellHardware : public Config::base_hardware_t {

private:

  emp::vector<double> endowments;
  FacingSet facing_set;
  Manager &man;

  const size_t pos;

public:

  CellHardware(
    emp::Ptr<emp::Random> local_rng,
    Config &cfg,
    Manager &man_,
    size_t pos_
  ) : Config::base_hardware_t(Config::base_hardware_t::DefaultEventLib(), local_rng)
    , endowments(cfg.NLEV()+1)
    , facing_set(*local_rng, cfg)
    , man(man_)
    , pos(pos_)
  {

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
