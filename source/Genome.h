#pragma once

#include "base/assert.h"
#include "hardware/EventDrivenGP.h"
#include "hardware/signalgp_utils.h"
#include "tools/Random.h"

#include "Config.h"

struct Genome {

  using hardware_t = Config::hardware_t;
  using program_t = Config::program_t;

  program_t program;

  Genome(const program_t & _p)
  : program(_p)
  { emp_assert(program.GetSize()); }

  Genome(
    emp::Random &local_rng,
    const Config::inst_lib_t &inst_lib,
    const Config &cfg
  ) : Genome(emp::GenRandSignalGPProgram<Config::TAG_WIDTH,Config::TRAIT_TYPE>(
        local_rng,
        inst_lib,
        cfg.PROGRAM_MIN_FUN_CNT(), cfg.PROGRAM_MAX_FUN_CNT(),
        cfg.PROGRAM_MIN_FUN_LEN(), cfg.PROGRAM_MAX_FUN_LEN(),
        cfg.PROGRAM_MIN_ARG_VAL(), cfg.PROGRAM_MAX_ARG_VAL()
    )) { emp_assert(program.GetSize()); }

    bool operator!=(const Genome& other) const {
      return program != other.program;
    }

};
