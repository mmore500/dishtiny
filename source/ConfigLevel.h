#pragma once

#include <limits>

#include "config/config.h"

EMP_BUILD_CONFIG(
  ConfigLevel,
  GROUP(DEFAULT, "Default settings for DISHTINY level"),
  VALUE(SIGNAL_RADIUS, int, std::numeric_limits<int>::max(), "How far can a signal propagate?"),
  VALUE(EVENT_RADIUS, int, 3, "TODO"),
  VALUE(HARVEST_VALUE, double, 6.0, "TODO"),
  VALUE(ACTIVATION_COST, double, -5.0, "TODO")
)
