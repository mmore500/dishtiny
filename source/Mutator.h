#pragma once

#include "hardware/signalgp_utils.h"

static constexpr size_t TAG_WIDTH = 16;

class Mutator : emp::SignalGPMutator<TAG_WIDTH> {

  public:
    Mutator(Dishtiny &config) : {
      // Setup SignalGP mutations.
      this->SetProgMinFuncCnt(config.PROGRAM_MIN_FUN_CNT());
      this->SetProgMaxFuncCnt(config.PROGRAM_MAX_FUN_CNT());
      this->SetProgMinFuncLen(config.PROGRAM_MIN_FUN_LEN());
      this->SetProgMaxFuncLen(config.PROGRAM_MAX_FUN_LEN());
      this->SetProgMinArgVal(config.PROGRAM_MIN_ARG_VAL());
      this->SetProgMaxArgVal(config.PROGRAM_MAX_ARG_VAL());
      this->SetProgMaxTotalLen(
          config.PROGRAM_MAX_FUN_CNT() * config.PROGRAM_MAX_FUN_LEN()
        );

      // Setup other SignalGP functions.
      this->ARG_SUB__PER_ARG(config.ARG_SUB__PER_ARG());
      this->INST_SUB__PER_INST(config.INST_SUB__PER_INST());
      this->INST_INS__PER_INST(config.INST_INS__PER_INST());
      this->INST_DEL__PER_INST(config.INST_DEL__PER_INST());
      this->SLIP__PER_FUNC(config.SLIP__PER_FUNC());
      this->FUNC_DUP__PER_FUNC(config.FUNC_DUP__PER_FUNC());
      this->FUNC_DEL__PER_FUNC(config.FUNC_DEL__PER_FUNC());
      this->TAG_BIT_FLIP__PER_BIT(config.TAG_BIT_FLIP__PER_BIT());
    }

}
