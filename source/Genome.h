#pragma once

#include "hardware/EventDrivenGP.h"
#include "tools/Random.h"

#include "CellHardware.h"

struct Genome {

  using program_t = CellHardware::Program program;

  emp::Random &local_rng;

  Genome(const program_t & _p, emp::Random &local_rng_)
  : program(_p)
  , local_rng(local_rng_)
  { ; }

  Genome(emp::Random &local_rng_, Config &cfg)
  : Genome(emp::GenRandSignalGPProgram(
        local_rng_,
        inst_lib /*TODO*/,
        cfg.PROGRAM_MIN_FUN_CNT(), cfg.PROGRAM_MAX_FUN_CNT(),
        cfg.PROGRAM_MIN_FUN_LEN(), cfg.PROGRAM_MAX_FUN_LEN(),
        cfg.PROGRAM_MIN_ARG_VAL(), cfg.PROGRAM_MAX_ARG_VAL()
      ), local_rng_)
  { ; }

};
