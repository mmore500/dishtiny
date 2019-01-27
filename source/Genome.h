#pragma once

#include "hardware/EventDrivenGP.h"
#include "tools/Random.h"

struct Genome {

  emp::EventDrivenGP_AW<TODO>::Program program;

  emp::Random &rng;

  Genome(const program_t & _p, emp::Random &rng_)
  : program(_p)
  , rng(rng_)
  { ; }

  Genome(emp::Random &rng_)
  : Genome(emp::GenRandSignalGPProgram(
        rng_,
        inst_lib,
        config.PROGRAM_MIN_FUN_CNT(), config.PROGRAM_MAX_FUN_CNT(),
        config.PROGRAM_MIN_FUN_LEN(), config.PROGRAM_MAX_FUN_LEN(),
        config.PROGRAM_MIN_ARG_VAL(), config.PROGRAM_MAX_ARG_VAL()
      ), rng_)
  { ; }

};
