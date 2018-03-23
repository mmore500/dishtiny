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
  VALUE(REP_THRESH, double, 8.0, "How much should replication cost?"),
  VALUE(ECOLOGICAL, bool, false, "Should we run in ecological mode?"),
  VALUE(KILL_THRESH, double, -11.0, "What is the minimum resource level required to stay alive?"),
  VALUE(CH_MAX, int, 4194304, "What is the maximum value of a channel identifier?"),
  VALUE(CULL_FREQ, size_t, 2500, "How frequently should culling take place?"),
  VALUE(GDATA_FREQ, size_t, 10000, "How frequently should we take genotype data?"),
  VALUE(CDATA_FREQ, size_t, 500000, "How often should we record a census of all the genotypes in the population?"),
  VALUE(PDATA_FREQ, size_t, 10000, "How frequently should we take phenotype data?"),
  VALUE(PUPDATE_FREQ, size_t, 100, "How frequently should we print the current update?"),
  VALUE(SUICIDE_EFF, double, 0.0, "How frequently should suicide attempts succeed?"),
  VALUE(RUN_LENGTH, size_t, 20000000, "How many updates should we run the experiment for?"),
  VALUE(SYSTEMATICS, bool, true, "Should we keep systematics data?"),
)
