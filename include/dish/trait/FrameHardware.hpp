#pragma once
#ifndef DISH_TRAIT_FRAMEHARDWARE_HPP_INCLUDE
#define DISH_TRAIT_FRAMEHARDWARE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/source/base/Ptr.h"
#include "../../../third-party/Empirical/source/base/vector.h"

#include "../config/Config.hpp"
#include "../datastructs/Membrane.hpp"
#include "../manager/Manager.hpp"
#include "../world/DishWorld.hpp"

#include "FrameHardware.hpp"

namespace dish {

class FrameCell;

class FrameHardware {

private:

  FrameCell &cell;

  emp::Random &local_rng;

  const Config &cfg;

  const size_t facing;

  bool inbox_active;

  double stockpile_reserve;
  size_t stockpile_reserve_fresh;

  double reproduction_reserve;
  size_t reproduction_reserve_fresh;

  Config::hardware_t cpu;

  size_t msg_dir;

  Membrane external_membrane;
  Membrane internal_membrane;

public:

  FrameHardware() = delete;

  FrameHardware(
      FrameCell &cell_,
      const size_t facing_,
      emp::Random &local_rng_,
      const Config &cfg_,
      const Config::inst_lib_t &inst_lib,
      const Config::event_lib_t &event_lib
    );

  FrameCell& Cell();

  const FrameCell& Cell() const;

  void Reset();

  double CheckStockpileReserve() const;

  void SetStockpileReserve(const double amt, const size_t dur);

  void TryClearStockpileReserve();

  double CheckReproductionReserve() const;

  void SetReproductionReserve(const double amt, const size_t dur);

  void TryClearReproductionReserve();

  void DispatchEnvTriggers(const size_t update);

  void SetupCompute(const size_t update);

  void StepProcess();

  void SetProgram(const Config::program_t & program);

  void PauseRepr(const size_t lev, const size_t dur);

  bool IsReprPaused(const size_t lev) const;

  size_t GetFacing() const;

  size_t GetMsgDir() const;

  void SetMsgDir(const size_t new_dir);

  void SetInboxActivity(const bool state);

  bool CheckInboxActivity() const;

  void QueueMessage(const Config::event_t &event);

  void QueueInternalMessage(const Config::event_t &event);

  void QueueInternalMessages(Config::inbox_t &inbox);

  void QueueExternalMessage(const Config::event_t &event);

  void QueueExternalMessages(Config::inbox_t &inbox);

  size_t CalcDir(const double relative_dir=0.0);

  bool IsLive(const int relative_dir=0);

  bool IsOccupied(const int relative_dir=0);

  bool IsCellChild(const int relative_dir=0);

  bool IsCellParent(const int relative_dir=0);

  bool IsChannelMate(const size_t lev, const int relative_dir=0);

  bool IsPropaguleChild(const int relative_dir=0);

  bool IsPropaguleParent(const int relative_dir=0);

  double IsPoorerThan(const int relative_dir=0);

  bool IsOlderThan(const int relative_dir=0);

  size_t IsExpired(const size_t lev, const int relative_dir=0);

  const Config::hardware_t& GetHardware();

  void SetRegulators(const Config::matchbin_t::state_t & target);

  void SetMatchBinState(const Config::matchbin_t::state_t & state);

  Membrane &GetExternalMembrane();

  Membrane &GetInternalMembrane();

};

} // namespace dish

#endif // #ifndef DISH_TRAIT_FRAMEHARDWARE_HPP_INCLUDE
