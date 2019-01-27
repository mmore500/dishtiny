#pragma once

#include <string>
#include <float.h>

#include "config/config.h"

EMP_BUILD_CONFIG(
  ConfigLevel,
  GROUP(DEFAULT, "Default settings for DISHTINY level"),
  VALUE(EVENT_VAL, double, 6.0, "TODO"),
  VALUE(EVENT_RADIUS, int, 3, "TODO"),
  VALUE(ACT_COST, double, -5.0, "TODO")
)
