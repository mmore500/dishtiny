#pragma once

#include <string>

#include "config/config.h"

EMP_BUILD_CONFIG(
  ConfigBase,

  GROUP(DEFAULT, "Default settings for DISHTINY"),
  VALUE(QUIESCENT_MAX, size_t, 4, "Length of quiescent period"),
  VALUE(NLEV, size_t, 2, "Number hierarchical resource levels"),
  VALUE(SEED, int, 0, "Random number seed (0 for based on time)"),
  VALUE(GRID_H, size_t, 120, "Height dimension of the grid"),
  VALUE(GRID_W, size_t, 120, "Width dimension of the grid"),
  VALUE(REP_THRESH, double, 8.0, "How much should replication cost?"),
  VALUE(START_RESOURCE, double, 7.0, "How resource should a cell start with?"),
  VALUE(ECOLOGICAL, bool, false, "Should we run in ecological mode?"),
  VALUE(KILL_THRESH, double, -11.0, "What is the minimum resource level required to stay alive?"),
  VALUE(CULL_FREQ, size_t, 50000, "How frequently should culling take place?"),
  VALUE(CULL_DELAY, size_t, 500000, "After when should the first cull take place?"),
  VALUE(GDATA_FREQ, size_t, 100000, "How frequently should we take genotype data?"),
  VALUE(CDATA_FREQ, size_t, 500000, "How often should we record a census of all the genotypes in the population and the channel distributions (as well as take other channel-related data)?"),
  VALUE(CHANMAP_DUR, size_t, 10, "Over how many updates should each channel map census record extend?"),
  VALUE(CHANMAP_TIMELAPSE_FREQ, size_t, 5552, "How frequently should we take one-off channel map snapshots (useful to create a timelapse)?"),
  VALUE(PDATA_FREQ, size_t, 10000, "How frequently should we take phenotype data?"),
  VALUE(PUPDATE_FREQ, size_t, 100, "How frequently should we print the current update?"),
  VALUE(SUICIDE_EFF, double, 0.90, "How frequently should suicide attempts succeed?"),
  VALUE(RUN_LENGTH, size_t, 25000000, "How many updates should we run the experiment for?"),
  VALUE(SYSTEMATICS, bool, true, "Should we keep systematics data?"),
  VALUE(SEED_POP, bool, false, "Should we seed the population?"),
  VALUE(SEED_POP_FILENAME, std::string, "ConsolidatedChampions.csv", "Where should we look for the genotypes to seed our population with?"),
  VALUE(SEED_POP_CLONECOUNT, size_t, 2, "How many clones of each champion genotype should we seed our population with?"),
  VALUE(CULL_TARGET, double, 0.05, "What proportion of the population should a cull remove?"),
  VALUE(CULL_PENALTY, double, -DBL_MAX, "What penalty should we inject into a channel pool?"),
  VALUE(RAINYDAY_PROPORTION, double, 2, "How much to have relative to endowment before reproducing and giving away one endowment?"),
  VALUE(HARDWARE_STEPS, size_t, 100, "How many hardware steps to run per update?")

  GROUP(SGP_PROGRAM_GROUP, "SignalGP program Settings"),
  VALUE(SGP_PROG_MAX_FUNC_CNT, size_t, 8, "Used for generating SGP programs. How many functions do we generate?"),
  VALUE(SGP_PROG_MIN_FUNC_CNT, size_t, 1, "Used for generating SGP programs. How many functions do we generate?"),
  VALUE(SGP_PROG_MAX_FUNC_LEN, size_t, 8, ".."),
  VALUE(SGP_PROG_MIN_FUNC_LEN, size_t, 1, ".."),
  VALUE(SGP_PROG_MAX_TOTAL_LEN, size_t, 256, "Maximum length of SGP programs."),
  GROUP(SGP_HARDWARE_GROUP, "SignalGP Hardware Settings"),
  VALUE(SGP_ENVIRONMENT_SIGNALS, bool, true, "Can environment signals trigger functions?"),
  VALUE(SGP_ACTIVE_SENSORS, bool, true, "Do agents have function active sensors?"),
  VALUE(SGP_HW_MAX_CORES, size_t, 16, "Max number of hardware cores; i.e., max number of simultaneous threads of execution hardware will support."),
  VALUE(SGP_HW_MAX_CALL_DEPTH, size_t, 128, "Max call depth of hardware unit"),
  VALUE(SGP_HW_MIN_BIND_THRESH, double, 0.0, "Hardware minimum referencing threshold"),
  GROUP(SGP_MUTATION_GROUP, "SignalGP Mutation Settings"),
  VALUE(SGP__PROG_MAX_ARG_VAL, int, 16, "Maximum argument value for instructions."),
  VALUE(SGP__PER_BIT__TAG_BFLIP_RATE, double, 0.005, "Per-bit mutation rate of tag bit flips."),
  VALUE(SGP__PER_INST__SUB_RATE, double, 0.005, "Per-instruction/argument subsitution rate."),
  VALUE(SGP__PER_INST__INS_RATE, double, 0.005, "Per-instruction insertion mutation rate."),
  VALUE(SGP__PER_INST__DEL_RATE, double, 0.005, "Per-instruction deletion mutation rate."),
  VALUE(SGP__PER_FUNC__SLIP_RATE, double, 0.05, "Per-function rate of slip mutations."),
  VALUE(SGP__PER_FUNC__FUNC_DUP_RATE, double, 0.05, "Per-function rate of function duplications."),
  VALUE(SGP__PER_FUNC__FUNC_DEL_RATE, double, 0.05, "Per-function rate of function deletions."),

  GROUP(LOGISTICS, "logistics"),
  VALUE(CONFIGLEVEL_BASENAME, std::string, "ConfigL", "Base filename")
  VALUE(CONFIGLEVEL_EXTENSION, std::string, "ConfigL", ".cfg")

)
