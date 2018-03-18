#pragma once

#include "config/config.h"

EMP_BUILD_CONFIG(
  DishtinyConfig,
  GROUP(DEFAULT, "Default settings for DISHTINY"),
  VALUE(QUIESCENT_MAX, size_t, 4, "Length of quiescent period"),
  VALUE(NLEV, size_t, 2, "Number hierarchical resource lebels"),
  VALUE(SEED, int, 0, "Random number seed (0 for based on time)"),
  VALUE(GRID_H, size_t, 120, "Height dimension of the grid"),
  VALUE(GRID_W, size_t, 120, "Width dimension of the grid"),
  VALUE(MAX_GENS, uint32_t, 2000, "How many generations should we process?"),
  VALUE(REP_THRESH, double, 8.0, "How much should replication cost?"),
  VALUE(KILL_THRESH, double, -11.0, "What is the minimum resource level required to stay alive?"),
  VALUE(CH_MAX, int, 4194304, "What is the maximum value of a channel identifier?"),
  VALUE(CULL_FREQ, size_t, 2500, "How frequently should culling take place?"),
  VALUE(GDATA_FREQ, size_t, 100, "How frequently should we take genotype data?"),
  VALUE(PDATA_FREQ, size_t, 100, "How frequently should we take phenotype data?"),
)
