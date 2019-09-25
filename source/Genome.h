#pragma once

#include "base/assert.h"
#include "hardware/EventDrivenGP.h"
#include "hardware/signalgp_utils.h"
#include "tools/Random.h"
#include "tools/Binomial.h"

#include "Mutator.h"
#include "Config.h"

class Genome {

  using hardware_t = Config::hardware_t;
  using program_t = Config::program_t;

  const Config &cfg;

  program_t program;
  emp::vector<Config::tag_t> tags;

public:

  Genome(const Genome &) = default;

  Genome(
    emp::Random &local_rng,
    const Config::inst_lib_t &inst_lib,
    const Config &cfg_
  ) : cfg(cfg_)
  , program(
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
    ))
  {
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
            inst_lib.GetName(inst.id) == "While" ||
            inst_lib.GetName(inst.id) == "Call" ||
            inst_lib.GetName(inst.id) == "Fork" ||
            inst_lib.GetName(inst.id) == "DuplicateCountdown" ||
            inst_lib.GetName(inst.id) == "DuplicateWhile" ||
            inst_lib.GetName(inst.id) == "DuplicateCall" ||
            inst_lib.GetName(inst.id) == "DuplicateFork" ||
            inst_lib.GetName(inst.id).find("Apoptosis") != std::string::npos
          ) {
            inst.id = nop_id;
          }
        }
      }
    }

    const program_t& GetProgram() const {
      return program;
    }

    void DoMutations(Mutator & mut, emp::Random & rand) {

      const static emp::Binomial bino(
        cfg.TAG_BIT_FLIP__PER_BIT(),
        Config::TAG_WIDTH
      );

      for (auto & t : tags) {
        t.Mutate(rand, bino.PickRandom(rand));
      }

      mut.ApplyMutations(program, rand);

    }

    const Config::tag_t& GetTag(emp::Random & rand, const size_t i) {
      while (tags.size() <= i) {
        tags.emplace_back(rand);
      }
      return tags[i];
    }

    bool operator!=(const Genome& other) const {
      return (
        program != other.program
        || tags != other.tags
      );
    }

    bool operator<(const Genome& other) const {
      return (
        (program < other.program)
        || ((program == other.program) && (tags < other.tags))
      );
    }

};
