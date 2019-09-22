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
  ) : Genome(
    emp::GenRandSignalGPProgram<
      Config::TAG_WIDTH,
      Config::TRAIT_TYPE,
      Config::hardware_t::matchbin_t
    >(
        local_rng,
        inst_lib,
        cfg.PROGRAM_MIN_FUN_CNT__INIT(), cfg.PROGRAM_MAX_FUN_CNT__INIT(),
        cfg.PROGRAM_MIN_FUN_LEN__INIT(), cfg.PROGRAM_MAX_FUN_LEN__INIT(),
        cfg.PROGRAM_MIN_ARG_VAL__INIT(), cfg.PROGRAM_MAX_ARG_VAL__INIT()
    )) {
      emp_assert(program.GetSize());

      // filter out some instructions from  initially generated genomes
      // by replacing them with NOPs
      const size_t nop_id = [&inst_lib](){
        for (size_t idx = 0; idx <inst_lib.GetSize(); ++idx) {
          if (inst_lib.GetName(idx) == "Nop") return idx;
        }
        emp_assert(false);
        return 0UL;
      }();

      for (auto & fun : program.program) {
        for (auto & inst : fun.inst_seq) {
          if (
            inst_lib.GetName(inst.id) == "Countdown" ||
            inst_lib.GetName(inst.id) == "If" ||
            inst_lib.GetName(inst.id) == "While" ||
            inst_lib.GetName(inst.id) == "Break" ||
            inst_lib.GetName(inst.id) == "Close" ||
            inst_lib.GetName(inst.id) == "Call" ||
            inst_lib.GetName(inst.id) == "Return" ||
            inst_lib.GetName(inst.id) == "Fork" ||
            inst_lib.GetName(inst.id) == "Terminate"
          ) {
            inst.id = nop_id;
          }
        }
      }
    }

    bool operator!=(const Genome& other) const {
      return program != other.program;
    }

    bool operator<(const Genome& other) const {
      return program < other.program;
    }

};
