#pragma once
#ifndef DISH2_CONFIG_CONFIGBASE_HPP_INCLUDE
#define DISH2_CONFIG_CONFIGBASE_HPP_INCLUDE

#include <limits>
#include <string>

#include "../../../third-party/conduit/include/uitsl/utility/UITSL_IF_WEB_ELSE.hpp"
#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/config/config.hpp"

#include "../spec/_NLEV.hpp"

namespace dish2 {

namespace internal {

using nlev_float_t = emp::array<float, DISH2_NLEV>;
using nreplev_float_t = emp::array<float, DISH2_NLEV + 1>;
using nlev_size_t_t = emp::array<size_t, DISH2_NLEV>;

} // namespace internal

EMP_BUILD_CONFIG(
  ConfigBase,

  GROUP(EXECUTION, "EXECUTION"),
  VALUE(N_THREADS, size_t, 1, "[NATIVE] How many threads should we run with?"),
  VALUE(RUN, bool, true,
    "[NATIVE] "
    "Should we run evolution or skip directly to post-processing and data " "collection?"
  ),
  VALUE(RUN_UPDATES, size_t, 0,
    "[NATIVE] How many updates should we run the experiment for?"
  ),
  VALUE(RUN_SECONDS, double, 0.0,
    "[NATIVE] How many seconds should we run the experiment for?"
  ),
  VALUE(MAIN_TIMEOUT_SECONDS, double, 13500.0,
    "[NATIVE] After how many seconds should we time out and fail with an error?"
  ),
  VALUE(END_SNAPSHOT_TIMEOUT_SECONDS, double, 1200.0,
    "[NATIVE] After how many seconds should the end snapshot timeout?"
  ),
  VALUE(LOG_FREQ, double, 20.0,
    "[NATIVE] How many seconds should pass between logging progress?"
  ),
  VALUE(ASYNCHRONOUS, size_t, 3,
    "[NATIVE] Should updates occur synchronously across threads and processes?"
  ),
  VALUE(SYNC_FREQ_MILLISECONDS, size_t, 100,
    "[NATIVE] "
    "How often updates occur synchronously across threads and processes for "
    "async mode 1?"
  ),
  VALUE(RNG_PRESEED, uint64_t, std::numeric_limits<uint64_t>::max(),
    "[NATIVE] Optionally override the calculated rng preseed."
  ),
  VALUE(THROW_ON_EXTINCTION, bool, true,
    "[NATIVE] Should we throw an exception if populations go extinct?"
  ),


  GROUP(EXPERIMENT, "EXPERIMENT"),
  VALUE(RUN_SLUG, std::string, "default", "Run-identifying slug"),
  VALUE(PHENOTYPIC_DIVERGENCE_N_UPDATES, size_t,
    UITSL_IF_WEB_ELSE(100, 2048),
    "How many updates should we run phenotypic divergence experiments for? "
    "If phenotypic divergence is not detected within this many updates, "
    "we consider two strains to be phenotypically identical."
  ),
  VALUE(PHENOTYPIC_DIVERGENCE_N_CELLS, size_t,
    100,
    "How many cells should we simulate while testing for phenotypic divergence?"
  ),
  VALUE(STINT, uint32_t,
    std::numeric_limits<uint32_t>::max(),
    "How many evolutionary stints have elapsed? "
    "Used to label output data and seed RNG."
  ),
  VALUE(SERIES, uint32_t,
    std::numeric_limits<uint32_t>::max(),
    "Which evolutionary series are we running? "
    "Used to label output data and seed RNG."
  ),
  VALUE(REPLICATE, std::string,
    "",
    "What replicate are we running? "
    "Used to label output data and seed RNG."
  ),
  VALUE(TREATMENT, std::string,
    "none",
    "What experimental treatment has been applied? "
    "Used to label output data."
  ),
  VALUE(
    SEED_FILL_FRACTION, double, 1.0,
    "If we are seeding the population, "
    "what fraction of available slots should we fill?"
  ),
  VALUE(
    GENESIS, std::string, "generate",
    "How should we initialize the population? "
    "Can be 'generate' to randomly generate a new population, "
    "'reconstitute' to load a population from file, "
    "'monoculture' to load a single genome from file, or "
    "'innoculate' to load genomes annotated with root ID keyname attributes "
    "from file."
  ),
  // VALUE(SEED_POP, bool, 0, "Should we seed the population?"),
  // VALUE(SEED_POP_ID, size_t, 0, "Should we seed the population with all seedpop IDs (0) or with a specific ID (>0)?"),
  // VALUE(SEED_MUTATIONS_P, float, 0.0, "With what probability should we apply mutations to seeded cells??"),


  GROUP(DEMOGRAPHICS, "DEMOGRAPHICS"),
  VALUE(N_CELLS, size_t,
    UITSL_IF_WEB_ELSE(3600, 10000), "How many cells should be simulated?"
  ),
  VALUE(WEAK_SCALING, bool, false, "[NATIVE] Should number of total cells be multiplied by the total number of threads (num procs times threads per proc)?"),
  VALUE(N_DIMS, size_t, DISH2_NLEV,
    "What dimensionality should the toroidal mesh have?"),
  VALUE(
    GROUP_EXPIRATION_DURATIONS,
    internal::nlev_size_t_t,
    (internal::nlev_size_t_t{ 256, 1024 }),
    "After how many /epochs/ should groups stop collecting resource?"
  ),
  VALUE(
    CELL_AGE_DURATION, size_t, 1024, "After how many epochs should cells die?"
  ),


  GROUP(RESOURCE, "RESOURCE"),
  VALUE(MIN_START_RESOURCE, float, 0.8f,
    "How much resource should a cell start with?"
  ),
  VALUE(MAX_START_RESOURCE, float, 0.9f,
    "How much resource should a cell start with?"
  ),
  VALUE(RESOURCE_DECAY, float, 0.995f,
    "How much resource should remain each update?"
  ),
  VALUE(APOP_RECOVERY_FRAC, float, 0.8f,
    "What fraction of REP_THRESH is recovered to heirs after apoptosis?"
  ),
  VALUE(SPAWN_DEFENSE_COST, float, 1.1f,
    "What is the cost of repelling an incoming spawn?"
  ),

  GROUP(HARVEST, "HARVEST"),
  VALUE(BASE_HARVEST_RATE, float, 0.02f,
    "How much resource should cells accrue per update?"
  ),
  VALUE(COLLECTIVE_HARVEST_RATE, internal::nlev_float_t,
    #if DISH2_NLEV == 1
    (internal::nlev_float_t{0.25f}),
    #elif DISH2_NLEV == 2
    (internal::nlev_float_t{0.25f, 0.25f}),
    #else
    (internal::nlev_float_t{}),
    #endif
    "How much resource should cells accrue per update?"
  ),
  VALUE(OPTIMAL_QUORUM_COUNT, internal::nlev_size_t_t,
    #if DISH2_NLEV == 1
    (internal::nlev_size_t_t{12}),
    #elif DISH2_NLEV == 2
    (internal::nlev_size_t_t{12, 12}),
    #else
    (internal::nlev_float_t{}),
    #endif
    "What group size does collective harvest work most effectively at?"
  ),

  GROUP(QUORUM, "QUORUM"),
  VALUE(P_SET_QUORUM_BIT, internal::nlev_float_t,
    #if DISH2_NLEV == 1
    (internal::nlev_float_t{1.0f}),
    #elif DISH2_NLEV == 2
    (internal::nlev_float_t{1.0f, 1.0f}),
    #else
    (internal::nlev_float_t{}),
    #endif
    "What fraction of cells should have a quorum bit set?"
  ),

  GROUP(QUORUM_CAPS, "QUORUM_CAPS"),
  VALUE(QUORUM_CAP, internal::nlev_size_t_t,
    #if DISH2_NLEV == 1
    (internal::nlev_size_t_t{12}),
    #elif DISH2_NLEV == 2
    (internal::nlev_size_t_t{12, 36}),
    #else
    (internal::nlev_float_t{}),
    #endif
    "At what quorum size should cell death be triggered?"
  ),
  VALUE(P_QUORUM_CAP_KILL, internal::nlev_float_t,
    #if DISH2_NLEV == 1
    (internal::nlev_float_t{0.0825f}),
    #elif DISH2_NLEV == 2
    (internal::nlev_float_t{0.0825f, 0.0825f}),
    #else
    (internal::nlev_float_t{}),
    #endif
    "With what probability should quorum death be enforced?"
  ),

  GROUP(GENOME, "GENOME"),
  VALUE(PROGRAM_START_SIZE, size_t, 128,
    "How many instructions should randomly-generated initial programs have?"
  ),
  VALUE(PROGRAM_MAX_SIZE, size_t, 4096,
    "How many instructions should programs be capped at?"
  ),
  VALUE(MUTATION_RATE, internal::nreplev_float_t,
    #if DISH2_NLEV == 1
    (internal::nreplev_float_t{0.1f, 1.0f}),
    #elif DISH2_NLEV == 2
    (internal::nreplev_float_t{0.1f, 0.1f, 1.0f}),
    #else
    (internal::nlev_float_t{}),
    #endif
    "For each replev, what fraction of cells should be mutated at all?"
  ),
  VALUE(POINT_MUTATION_RATE, float, 0.0002f,
    "What fraction of bits should be scrambled?"
  ),
  VALUE(SEQUENCE_DEFECT_RATE, float, 0.001f,
    "How often should sloppy copy defect occur?"
  ),
  VALUE(MINOR_SEQUENCE_MUTATION_BOUND, size_t,
    8,
    "For minor sequence mutations, "
    "at most how many instructions should be inserted or deleted?"
  ),
  VALUE(SEVERE_SEQUENCE_MUTATION_RATE, float,
    0.001f,
    "TODO"
  ),


  GROUP(HARDWARE, "HARDWARE"),
  VALUE(HARDWARE_EXECUTION_ROUNDS, size_t, 1,
    "How many hardware cardinal rounds to run?"
  ),
  VALUE(HARDWARE_EXECUTION_CYCLES, size_t, 16,
    "How many hardware cycles to run per round?"
  ),
  VALUE(CONTROLLER_MAPPED_STATE_DEFECT_RATE, float, 0.0005f,
    "At what rate should bits should be flipped in writable memory?"
  ),


  GROUP(SERVICES, "SERVICES"),
  VALUE(APOPTOSIS_SERVICE_FREQUENCY, size_t, 16,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(BIRTH_SETUP_SERVICE_FREQUENCY, size_t, 16,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(CELL_AGE_SERVICE_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(CONDUIT_FLUSH_SERVICE_FREQUENCY, size_t, 16,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(COLLECTIVE_HARVESTING_SERVICE_FREQUENCY, size_t, 4,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(CPU_EXECUTION_SERVICE_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(GROUP_EXPIRATION_SERVICE_FREQUENCY, size_t, 64,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(RUNNING_LOG_PURGE_SERVICE_FREQUENCY, size_t, 64,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(DIVERSITY_MAINTENANCE_SERVICE_FREQUENCY, size_t, 8,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(DIVERSITY_MAINTENANCE_PREVALENCE, double, 0.25,
    "If an originally-seeded ancestor's descendants constitute more than "
    "this fraction of the population, decay their resource stockpiles."
  ),
  VALUE(STINT_DIVERSITY_MAINTENANCE_SERVICE_FREQUENCY, size_t, 0,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(STINT_DIVERSITY_MAINTENANCE_PREVALENCE, double, 0.25,
    "If a seeded or reconstituted stint-originating ancestor's descendants "
    "constitute more than this fraction of the population, "
    "decay their resource stockpiles."
  ),
  VALUE(DECAY_TO_BASELINE_SERVICE_FREQUENCY, size_t, 32,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(EPOCH_ADVANCE_SERVICE_FREQUENCY, size_t, 8,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(EVENT_LAUNCHING_SERVICE_FREQUENCY, size_t, 8,
    "Run service every ?? updates. Must be > 1."
  ),
  VALUE(INTERMITTENT_CPU_RESET_SERVICE_FREQUENCY, size_t, 64,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(INTERMITTENT_STATE_PERTURB_SERVICES_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(INTER_MESSAGE_COUNTER_CLEAR_SERVICE_FREQUENCY, size_t, 16,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(INTER_MESSAGE_LAUNCHING_SERVICE_FREQUENCY, size_t, 8,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(INTRA_MESSAGE_LAUNCHING_SERVICE_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(STATE_OUTPUT_PUT_SERVICE_FREQUENCY, size_t, 8,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(PUSH_SERVICE_FREQUENCY, size_t, 16,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(QUORUM_CAP_SERVICE_FREQUENCY, size_t, 16,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(QUORUM_SERVICE_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(RESOURCE_DECAY_SERVICE_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(RESOURCE_HARVESTING_SERVICE_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(RESOURCE_INPUT_JUMP_SERVICE_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(RESOURCE_RECEIVING_SERVICE_FREQUENCY, size_t, 4,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(RESOURCE_SENDING_SERVICE_FREQUENCY, size_t, 1,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(SPAWN_SENDING_SERVICE_FREQUENCY, size_t, 16,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(STATE_INPUT_JUMP_SERVICE_FREQUENCY, size_t, 8,
    "Run service every ?? updates. Must be a power of 2."
  ),
  VALUE(CONTROLLER_MAPPED_STATE_NOISE_SERVICE_FREQUENCY, size_t, 8,
    "Run service every ?? updates. Must be a power of 2."
  ),


  GROUP(DATA, "DATA"),
  VALUE(PHENOTYPE_EQUIVALENT_NOPOUT, bool, false,
    "[NATIVE] "
    "Should we make and record a phenotype equivalent nopout strain at the end "
    "of the run? "
    "Must also enable ARTIFACTS_DUMP."
  ),
  VALUE(BATTLESHIP_PHENOTYPE_EQUIVALENT_NOPOUT, bool, false,
    "[NATIVE] Should we make and record a phenotype equivalent nopout strain at the end of the run? Must also enable ARTIFACTS_DUMP."
  ),
  VALUE(JENGA_PHENOTYPE_EQUIVALENT_NOPOUT, bool, false,
    "[NATIVE] "
    "Should we make and record a phenotype equivalent nopout strain at the end "
    "of the run? "
    "Must also enable ARTIFACTS_DUMP."
  ),
  VALUE(JENGA_NOP_OUT_SAVE_PROGRESS_AND_QUIT_SECONDS, size_t, 10800,
    "[NATIVE] After how many seconds should we save nop-out progress and quit?"
  ),

  VALUE(TEST_INTERROOT_PHENOTYPE_DIFFERENTIATION, bool, false,
    "[NATIVE] Should we test for phenotype differentiation between roots?"
  ),
  VALUE(ALL_DRAWINGS_WRITE, bool, false,
    "[NATIVE] "
    "Should we generate and record drawings of the final state of the "
    "simulation? Must also enable DATA_DUMP."
  ),
  VALUE(DATA_DUMP, bool, false,
    "[NATIVE] Should we record data on the final state of the simulation?"
  ),
  VALUE(RUNNINGLOGS_DUMP, bool, false,
    "[NATIVE] "
    "Should we dump running logs at the end of the simulation? "
    "Must also enalbe DATA_DUMP."
  ),
  VALUE(CENSUS_WRITE, bool, false,
    "[NATIVE] Should we write the cell census at the end of the simulation? Must also enalbe DATA_DUMP."
  ),
  VALUE(ARTIFACTS_DUMP, bool, false,
    "[NATIVE] Should we record data on the final state of the simulation?"
  ),
  VALUE(BENCHMARKING_DUMP, bool, false,
    "[NATIVE] Should we record data for benchmarking the simulation?"
  ),
  VALUE(ROOT_ABUNDANCES_FREQ, size_t, 0,
    "[NATIVE] "
    "How many updates should elapse between recording phylogenetic root "
    "abundances? "
    "If 0, never record phylogenetic root abundances. Must be power of two."
  ),
  VALUE(ABORT_IF_COALESCENT_FREQ, size_t, 0,
    "[NATIVE] "
    "How many updates should elapse between checking for coalescence? "
    "If 0, never check for coalescence. Must be power of two."
  ),
  VALUE(ABORT_IF_EXTINCT_FREQ, size_t, 0,
    "[NATIVE] "
    "How many updates should elapse between checking for coalescence? "
    "If 0, never check for coalescence. Must be power of two."
  ),
  VALUE(ABORT_AT_LIVE_CELL_FRACTION, double, 0.0,
    "[NATIVE] Should we terminate once a live cell fraction is reached? "
    "If 0, will not terminate."
  ),
  VALUE(REGULATION_VIZ_CLAMP, double, 10.0,
    "What bounds should we clamp regulation values into before running PCA "
    "visualization?"
  ),
  VALUE(RUNNING_LOG_DURATION, size_t, 4,
    "How many purge epochs should we keep events in the running log?"
  ),
  VALUE(SELECTED_DRAWINGS_FREQ, size_t, 0,
    "How often should we draw the current world?"
  ),
  VALUE(DRAWING_WIDTH_PX, double, 500.0,
    "What should the width of the drawings be, in pixels?"
  ),
  VALUE(DRAWING_HEIGHT_PX, double, 500.0,
    "What should the height of the drawings be, in pixels?"
  ),
  VALUE(SELECTED_DRAWINGS, std::string, "",
    "What drawings should be drawn? "
    "Provide slugified drawer names seperated by colons."
  ),
  VALUE(ANIMATE_FRAMES, bool, false,
    "Should we stich the output images into a video? "
    "Only valid if DRAWING_FREQ is not 0."
  ),
  VALUE(VIDEO_FPS, size_t, 16,
    "How many frames per second should the video be?"
  ),
  VALUE(VIDEO_MAX_FRAMES, size_t, 960,
    "At most how many frames should the video include?"
  ),

)

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_CONFIGBASE_HPP_INCLUDE
