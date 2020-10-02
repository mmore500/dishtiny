#pragma once
#ifndef DISH_GENOME_HPP_INCLUDE
#define DISH_GENOME_HPP_INCLUDE

#include "../../third-party/Empirical/source/base/assert.h"
#include "../../third-party/Empirical/source/hardware/EventDrivenGP.h"
#include "../../third-party/Empirical/source/hardware/signalgp_utils.h"
#include "../../third-party/Empirical/source/tools/Random.h"
#include "../../third-party/Empirical/source/tools/Distribution.h"

#include "Mutator.hpp"
#include "Config.hpp"

namespace dish {

class Genome {

  using hardware_t = Config::hardware_t;
  using program_t = Config::program_t;

  const Config &cfg;

  program_t program;
  program_t program_spiker;
  emp::vector<Config::tag_t> tags;
  size_t root_id;

public:

  Genome(const Genome&) = default;

  Genome& operator=(const Genome&) = delete;

  Genome(
    const Config& cfg_,
    const Config::inst_lib_t &ilib,
    const Config::inst_lib_t &ilib_spiker
  ) : cfg(cfg_)
  , program(&ilib)
  , program_spiker(&ilib_spiker)
  { }

  Genome(
    const Config &cfg_,
    const program_t &program_,
    const program_t &program_spiker_,
    const emp::vector<Config::tag_t> &tags_,
    const size_t root_id_
  ) : cfg(cfg_)
  , program(program_)
  , program_spiker(program_spiker_)
  , tags(tags_)
  , root_id(root_id_) { ; }

  Genome(
    emp::Random &local_rng,
    const Config::inst_lib_t &inst_lib,
    const Config::inst_lib_t &inst_lib_spiker,
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
  , program_spiker(
    emp::GenRandSignalGPProgram<
      Config::TAG_WIDTH,
      Config::TRAIT_TYPE,
      Config::hardware_t::matchbin_t
    >(
        local_rng,
        inst_lib_spiker,
        cfg.PROGRAM_MIN_FUN_CNT__INIT(), cfg.PROGRAM_MAX_FUN_CNT__INIT(),
        cfg.PROGRAM_MIN_FUN_LEN__INIT(), cfg.PROGRAM_MAX_FUN_LEN__INIT(),
        cfg.PROGRAM_MIN_ARG_VAL__INIT(), cfg.PROGRAM_MAX_ARG_VAL__INIT()
    ))
  {
      emp_assert(program.GetSize());

      for (size_t i = 0; i < 14 + 6 * cfg.NLEV(); ++i) {
        tags.emplace_back(local_rng);
      }

      // filter out some instructions from  initially generated genomes
      // by replacing them with NOPs
      const size_t nop_id = [&inst_lib](){
        for (size_t idx = 0; idx <inst_lib.GetSize(); ++idx) {
          if (inst_lib.GetName(idx) == "Nop") return idx;
        }
        emp_assert(false);
        return 0UL;
      }();

      for (auto & fun : program) {
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
            inst_lib.GetName(inst.id).find("Apoptosis") != std::string::npos ||
            inst_lib.GetName(inst.id).find("Prune") != std::string::npos
          ) {
            inst.id = nop_id;
          }
        }
      }

      // filter out some instructions from  initially generated genomes
      // by replacing them with NOPs
      const size_t nop_id_spiker = [&inst_lib_spiker](){
        for (size_t idx = 0; idx <inst_lib_spiker.GetSize(); ++idx) {
          if (inst_lib_spiker.GetName(idx) == "Nop") return idx;
        }
        emp_assert(false);
        return 0UL;
      }();

      for (auto & fun : program_spiker) {
        for (auto & inst : fun.inst_seq) {
          if (
            inst_lib_spiker.GetName(inst.id) == "Countdown" ||
            inst_lib_spiker.GetName(inst.id) == "While" ||
            inst_lib_spiker.GetName(inst.id) == "Call" ||
            inst_lib_spiker.GetName(inst.id) == "Fork" ||
            inst_lib_spiker.GetName(inst.id) == "DuplicateCountdown" ||
            inst_lib_spiker.GetName(inst.id) == "DuplicateWhile" ||
            inst_lib_spiker.GetName(inst.id) == "DuplicateCall" ||
            inst_lib_spiker.GetName(inst.id) == "DuplicateFork" ||
            inst_lib_spiker.GetName(inst.id).find("Apoptosis") != std::string::npos ||
            inst_lib_spiker.GetName(inst.id).find("Prune") != std::string::npos
          ) {
            inst.id = nop_id_spiker;
          }
        }
      }
    }

    const program_t& GetProgram() const {
      return program;
    }

    const program_t& GetProgramSpiker() const {
      return program_spiker;
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
      mut.ApplyMutations(program_spiker, rand);

    }

    const Config::tag_t& GetTag(const size_t i) { return tags[i]; }

    const emp::vector<Config::tag_t> & GetTags() const { return tags; }

    bool operator==(const Genome& other) const {
      return (
        program == other.program
        && program_spiker == other.program_spiker
        && tags == other.tags
      );
    }

    bool operator!=(const Genome& other) const {
      return !(*this == other);
    }

    bool operator<(const Genome& other) const {
      return std::tie(program, program_spiker, tags)
        < std::tie(other.program, other.program_spiker, other.tags);
    }

    size_t GetRootID() const {
      return root_id;
    }

    void SetRootID(const size_t id) {
      root_id = id;
    }

    template <class Archive>
    void serialize( Archive & ar ) {
      ar(
        CEREAL_NVP(program),
        CEREAL_NVP(program_spiker),
        CEREAL_NVP(tags),
        CEREAL_NVP(root_id)
      );
    }

};

} // namespace dish

#endif // #ifndef DISH_GENOME_HPP_INCLUDE
