#pragma once

#include "hardware/EventDrivenGP.h"
#include "hardware/signalgp_utils.h"
#include "tools/Random.h"

#include "CellHardware.h"
#include "InstructionLib.h"

struct Genome {

  using program_t = CellHardware::program_t;

  program_t program;

  Genome(const program_t & _p)
  : program(_p)
  { ; }

  Genome(emp::Random &local_rng, Config &cfg)
  : Genome(emp::GenRandSignalGPProgram(
        local_rng,
        *CellHardware::DefaultInstLib() /* TODO */,
        cfg.PROGRAM_MIN_FUN_CNT(), cfg.PROGRAM_MAX_FUN_CNT(),
        cfg.PROGRAM_MIN_FUN_LEN(), cfg.PROGRAM_MAX_FUN_LEN(),
        cfg.PROGRAM_MIN_ARG_VAL(), cfg.PROGRAM_MAX_ARG_VAL()
      ))
  { ; }

};
