#pragma once

#include <algorithm>

#include "base/vector.h"
#include "base/Ptr.h"

#include "Config.h"
#include "FrameHardware.h"
#include "Manager.h"
#include "DishWorld.h"

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

  Config::matchbin_t membrane;
  Config::matchbin_t internal_membrane;

  std::unordered_map<
    Config::matchbin_t::tag_t,
    Config::matchbin_t::uid_t
  > membrane_tags;

  std::unordered_map<
    Config::matchbin_t::tag_t,
    Config::matchbin_t::uid_t
  > internal_membrane_tags;

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

  void QueueMessages(Config::inbox_t &inbox);

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

  void SetRegulators(Config::matchbin_t & target);

  Config::matchbin_t &GetMembrane();

  std::unordered_map<
    Config::matchbin_t::tag_t,
    Config::matchbin_t::uid_t
  > &GetMembraneTags();

  Config::matchbin_t &GetInternalMembrane();

  std::unordered_map<
    Config::matchbin_t::tag_t,
    Config::matchbin_t::uid_t
  > &GetInternalMembraneTags();

};
