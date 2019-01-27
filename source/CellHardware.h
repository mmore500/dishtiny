#pragma once

#include "hardware/EventDrivenGP.h"

#include "Cardi.h"
#include "FacingSet.h"

static constexpr size_t TAG_WIDTH = 16;

using base_hardware_t = public emp::EventDrivenGP_AW<TAG_WIDTH>;

class CellHardware : public base_hardware_t {

private:

  emp::vector<double> endowments;
  FacingSet facings;

  size_t pos;

public:

  CellHardware(
    emp::Random *rng,
    emp::DishtinyConfig &config,
    size_t pos_
  ) :
    , endowments(config.NLEV()+1)
    , FacingSet(config)
    , pos(pos_)
    , base_hardware_t(inst_lib, emp::base_hardware_t::DefaultEventLib(), rng)
  {

  }

  const size_t GetPos() {
    return pos;
  }

  FacingSet& GetFacing() {
    return facings;
  }

  Tile& GetTile() {
    return tile;
  }

};
