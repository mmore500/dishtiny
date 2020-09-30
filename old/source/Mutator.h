#pragma once

#include "hardware/signalgp_utils.h"

#include "Config.h"

class Mutator : public emp::SignalGPMutator<
  Config::TAG_WIDTH,
  Config::TRAIT_TYPE,
  Config::hardware_t::matchbin_t
> {

  public:
    Mutator(const Config &cfg) {
      // Setup SignalGP mutations.
      this->SetProgMinFuncCnt(cfg.PROGRAM_MIN_FUN_CNT());
      this->SetProgMaxFuncCnt(cfg.PROGRAM_MAX_FUN_CNT());
      this->SetProgMinFuncLen(cfg.PROGRAM_MIN_FUN_LEN());
      this->SetProgMaxFuncLen(cfg.PROGRAM_MAX_FUN_LEN());
      this->SetProgMinArgVal(cfg.PROGRAM_MIN_ARG_VAL());
      this->SetProgMaxArgVal(cfg.PROGRAM_MAX_ARG_VAL());
      this->SetProgMaxTotalLen(
          cfg.PROGRAM_MAX_FUN_CNT() * cfg.PROGRAM_MAX_FUN_LEN()
        );

      // Setup other SignalGP functions.
      this->ARG_SUB__PER_ARG(cfg.ARG_SUB__PER_ARG());
      this->INST_SUB__PER_INST(cfg.INST_SUB__PER_INST());
      this->INST_INS__PER_INST(cfg.INST_INS__PER_INST());
      this->INST_DEL__PER_INST(cfg.INST_DEL__PER_INST());
      this->SLIP__PER_FUNC(cfg.SLIP__PER_FUNC());
      this->FUNC_DUP__PER_FUNC(cfg.FUNC_DUP__PER_FUNC());
      this->FUNC_DEL__PER_FUNC(cfg.FUNC_DEL__PER_FUNC());
      this->TAG_BIT_FLIP__PER_BIT(cfg.TAG_BIT_FLIP__PER_BIT());
    }

};
