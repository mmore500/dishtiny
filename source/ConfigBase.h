#pragma once

#include <float.h>
#include <string>

#include "config/config.h"

EMP_BUILD_CONFIG(
  ConfigBase,

  GROUP(CORE, "Core settings for DISHTINY"),
  VALUE(QUIESCENT_MAX, size_t, 9, "Length of quiescent period after activation during resource collection."),
  VALUE(NLEV, size_t, 2, "Number of hierarchical resource levels."),
  VALUE(SEED, int, 1, "Random number generator seed."),
  VALUE(GRID_H, size_t, 60, "How many tiles tall should the grid be?"),
  VALUE(GRID_W, size_t, 60, "How many tiles wide should the grid be?"),
  VALUE(RESOURCE_DECAY, double, 0.999, "How much resource should remain each update?"),
  VALUE(REP_THRESH, double, 4.0, "How much should replication cost?"),
  VALUE(START_RESOURCE, double, 3.0, "How much resource should a cell start with?"),
  VALUE(KILL_THRESH, double, -5.0, "What is the minimum resource level required to stay alive?"),
  VALUE(APOP_RECOVERY_FRAC, double, 0.8, "What fraction of REP_THRESH is recovered to heirs after apoptosis?"),
  VALUE(BASE_RESOURCE_INFLOW, double, 0.0, "What amount of resource should be provided to cells at each update?"),
  VALUE(AGE_LIMIT_MULTIPLIER, double, 1.5, "What ratio of EVENT_RADIUS should the limit on cell age be?"),
  VALUE(CHANNELS_VISIBLE, bool, true, "Should channels have any effect in the instruction set and event triggers?"),
  VALUE(GEN_INCR_FREQ, size_t, 512, "How often should we increase cell generation counters?"),
  VALUE(EXP_GRACE_PERIOD, double, 3.0, "How many channel generations should resource collection be allowed after a cell's expires channel generation counter?"),

  GROUP(PROGRAM_GROUP, "SignalGP program Settings"),
  VALUE(PROGRAM_MAX_FUN_CNT, size_t, 48, "Used for generating SGP programs. At most, how many functions do we generate?"),
  VALUE(PROGRAM_MIN_FUN_CNT, size_t, 4, "Used for generating SGP programs. At least, how many functions do we generate?"),
  VALUE(PROGRAM_MAX_FUN_LEN, size_t, 16, "Used for generating SGP programs. At most, for each function how many instructions do we generate?"),
  VALUE(PROGRAM_MIN_FUN_LEN, size_t, 1, "Used for generating SGP programs. At least, for each function how many instructions do we generate?"),

  GROUP(HARDWARE_GROUP, "SignalGP Hardware Settings"),
  VALUE(HARDWARE_STEPS, size_t, 4, "How many hardware steps to run per update?"),
  VALUE(ENVIRONMENT_SIGNALS, bool, true, "Can environment signals trigger functions?"),
  VALUE(ACTIVE_SENSORS, bool, true, "Do agents have function active sensors?"),
  VALUE(HW_MAX_CORES, size_t, 16, "Max number of hardware cores; i.e., max number of simultaneous threads of execution hardware will support."),
  VALUE(HW_MAX_CALL_DEPTH, size_t, 128, "Max call depth of a hardware unit."),
  VALUE(INBOX_CAPACITY, size_t, 16, "Capacity of a cell's messaging inbox."),
  VALUE(ENV_TRIG_FREQ, size_t, 32, "How often to fire environmental trigger events?"),
  VALUE(COMPUTE_FREQ, size_t, 8, "How often to step the CPUs?"),

  GROUP(MUTATION_GROUP, "SignalGP Mutation Settings"),
  VALUE(MUTATION_RATE, double, 0.01, "What percentage of offspring should experience mutations?"),
  VALUE(PROPAGULE_MUTATION_RATE, double, 0.0, "What percentage of propagule offspring should experience mutations?"),
  VALUE(PROGRAM_MIN_ARG_VAL, int, 0, "Minimum argument value for instructions."),
  VALUE(PROGRAM_MAX_ARG_VAL, int, 4, "Maximum argument value for instructions."),
  VALUE(TAG_BIT_FLIP__PER_BIT, double, 0.005, "Per-bit mutation rate of tag bit flips."),
  VALUE(INST_SUB__PER_INST, double, 0.005, "Per-instruction/argument subsitution mutation rate."),
  VALUE(ARG_SUB__PER_ARG, double, 0.005, "Per-instruction/argument subsitution mutation rate."),
  VALUE(INST_INS__PER_INST, double, 0.005, "Per-instruction insertion mutation rate."),
  VALUE(INST_DEL__PER_INST, double, 0.005, "Per-instruction deletion mutation rate."),
  VALUE(SLIP__PER_FUNC, double, 0.05, "Per-function rate of slip mutations."),
  VALUE(FUNC_DUP__PER_FUNC, double, 0.05, "Per-function rate of function duplication mutations."),
  VALUE(FUNC_DEL__PER_FUNC, double, 0.05, "Per-function rate of function deletion mutationss."),

  GROUP(LOGISTICS, "logistics"),
  VALUE(TREATMENT_DESCRIPTOR, std::string, "unspecified", "[NATIVE] Treatment identifying slug"),
  VALUE(CONFIGLEVEL_BASENAME, std::string, "level=", "[NATIVE] Base filename"),
  VALUE(CONFIGLEVEL_EXTENSION, std::string, ".cfg", "[NATIVE] Filename extension"),
  VALUE(SNAPSHOT_FREQUENCY, size_t, 100000, "[NATIVE] How often should we save data snapshots?"),
  VALUE(SNAPSHOT_LENGTH, size_t, 256, "[NATIVE] How long should snapshots last for?"),
  VALUE(RUN_LENGTH, size_t, 10000000, "[NATIVE] How many updates should we run the experiment for?"),
  VALUE(SYSTEMATICS, bool, true, "[NATIVE] Should we keep systematics data?"),
  VALUE(SEED_POP, bool, false, "[NATIVE] Should we seed the population?; TOOD unimplemented"),
  VALUE(SEED_POP_FILENAME, std::string, "ConsolidatedChampions.csv", "[NATIVE] Where should we look for the genotypes to seed our population with?; TODO unimplemented"),
  VALUE(SEED_POP_CLONECOUNT, size_t, 2, "[NATIVE] How many clones of each champion genotype should we seed our population with?; TODO unimplemented"),
  VALUE(ANIMATION_FREQUENCY, size_t, 250, "[NATIVE] How often should we save data that will be used to make animations?")

)
