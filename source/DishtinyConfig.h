#pragma once

#include <string>

#include "config/config.h"

EMP_BUILD_CONFIG(
  DishtinyConfig,
  GROUP(DEFAULT, "Default settings for DISHTINY"),
  VALUE(QUIESCENT_MAX, size_t, 4, "Length of quiescent period"),
  VALUE(NLEV, size_t, 2, "Number hierarchical resource levels"),
  VALUE(SEED, int, 0, "Random number seed (0 for based on time)"),
  VALUE(GRID_H, size_t, 120, "Height dimension of the grid"),
  VALUE(GRID_W, size_t, 120, "Width dimension of the grid"),
  VALUE(REP_THRESH, double, 8.0, "How much should replication cost?"),
  VALUE(ECOLOGICAL, bool, false, "Should we run in ecological mode?"),
  VALUE(KILL_THRESH, double, -11.0, "What is the minimum resource level required to stay alive?"),
  VALUE(CH_MAX, int, 4194304, "What is the maximum value of a channel identifier?"),
  VALUE(CULL_FREQ, size_t, 2500, "How frequently should culling take place?"),
  VALUE(GDATA_FREQ, size_t, 100000, "How frequently should we take genotype data?"),
  VALUE(CDATA_FREQ, size_t, 500000, "How often should we record a census of all the genotypes in the population and the channel distributions (as well as take other channel-related data)?"),
  VALUE(CHANMAP_DUR, size_t, 10, "Over how many updates should each channel map census record extend?"),
  VALUE(CHANMAP_TIMELAPSE_FREQ, size_t, 5552, "How frequently should we take one-off channel map snapshots (useful to create a timelapse)?"),
  VALUE(PDATA_FREQ, size_t, 10000, "How frequently should we take phenotype data?"),
  VALUE(PUPDATE_FREQ, size_t, 100, "How frequently should we print the current update?"),
  VALUE(SUICIDE_EFF, double, 0.8, "How frequently should suicide attempts succeed?"),
  VALUE(RUN_LENGTH, size_t, 25000000, "How many updates should we run the experiment for?"),
  VALUE(SYSTEMATICS, bool, true, "Should we keep systematics data?"),
  VALUE(SEED_POP, bool, false, "Should we seed the population?"),
  VALUE(SEED_POP_FILENAME, std::string, "ConsolidatedChampions.csv", "Where should we look for the genotypes to seed our population with?"),
  VALUE(SEED_POP_CLONECOUNT, size_t, 2, "How many clones of each champion genotype should we seed our population with?")
)
