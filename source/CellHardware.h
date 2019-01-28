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
    emp::Random &local_rng,
    Config &cfg,
    Manager &man_,
    size_t pos_
  ) :
    , endowments(cfg.NLEV()+1)
    , facing_set(cfg)
    , pos(pos_)
    , man(man_)
    , base_hardware_t(inst_lib, emp::base_hardware_t::DefaultEventLib(), &local_rng)
  {

  }

  const size_t GetPos() {
    return pos;
  }

  FacingSet& GetFacingSet() {
    return facing_set;
  }

  Manage& GetManager() {
    return man;
  }

};
