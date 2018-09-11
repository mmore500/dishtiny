#pragma once

#include "DishtinyConfig.h"
#include "GridSpec.h"

class CustomConfig {
public:
  const double EVENT_VALS[2] = {6.0, 6.0};
  const int EVENT_RADII[2] = {3, 24};
  const double V_COST[2] = {-5.0, -5.0};
  const double CULL_PENALTY;
  const double PM_OFF_CH_CAP[2] {0.00005, 0.00005};
  const double PM_RES_POOL[3] {0.00005, 0.00005, 0.00005};
  const double PM_AVOID_OVER[2] {0.00005, 0.00005};
  const double PM_SORT_OFF[2] {0.00005, 0.00005};
  const double PM_DAMAGE_SUICIDE[3] {0.00005, 0.00005, 0.00005};
  const double PM_ENDOWMENT[3] {0.00005, 0.00005, 0.00005};

  CustomConfig(DishtinyConfig & dconfig, GridSpec & spec)
  : CULL_PENALTY(5.0 * (spec.area) * dconfig.KILL_THRESH())
  { ; }

};
