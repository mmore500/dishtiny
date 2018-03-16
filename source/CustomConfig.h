#pragma once

#include "DishtinyConfig.h"
#include "GridSpec.h"

class CustomConfig {
public:
  const double EVENT_VALS[2] = {6.0, 6.0};
  const int EVENT_RADII[2] = {4, 12};
  const double V_COST[2] = {-5.0, -5.0};
  const double CULL_TARGET;
  const double CULL_PENALTY;
  const double PM_OFF_CH_CAP[2] {0.00001, 0.00001};
  const double PM_RES_POOL[3] {0.00001, 0.00001, 0.00001};
  const double PM_OFF_OVER[3] {0.00001, 0.00001};
  const double PM_ENDOWMENT[3] {0.00001, 0.00001, 0.00001};

  CustomConfig(DishtinyConfig & dconfig, GridSpec & spec)
  : CULL_TARGET(2.0 / (spec.area))
  , CULL_PENALTY(5.0 * (spec.area) * dconfig.KILL_THRESH())
  { ; }

};
