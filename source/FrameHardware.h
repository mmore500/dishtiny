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

  const Config &cfg;

  const size_t facing;

  bool inbox_active;

  double stockpile_reserve;

  emp::Ptr<emp::vector<bool>> repr_pause_cur;
  emp::Ptr<emp::vector<bool>> repr_pause_nxt;

  Config::hardware_t cpu;

  size_t msg_dir;

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

  ~FrameHardware();

  FrameCell& Cell();

  void Reset();

  double CheckStockpileReserve() const;

  void SetStockpileReserve(const double amt);

  void DispatchEnvTriggers();

  void SetupCompute();

  void StepProcess();

  void SetProgram(const Config::program_t & program);

  void PauseRepr(const size_t lev);

  bool IsReprPaused(const size_t lev) const;

  size_t GetFacing() const;

  size_t GetMsgDir() const;

  void SetMsgDir(const size_t new_dir);

  void SetInboxActivity(const bool state);

  bool CheckInboxActivity() const;

  void QueueMessage(const Config::event_t &event);

  void QueueMessages(Config::inbox_t &inbox);

  size_t CalcDir(const double relative_dir);

  bool IsLive(const int relative_dir=0);

  bool IsOccupied(const int relative_dir=0);

  bool IsCellChild(const int relative_dir=0);

  bool IsCellParent(const int relative_dir=0);

  bool IsChannelMate(const size_t lev, const int relative_dir=0);

  bool IsPropaguleChild(const int relative_dir=0);

  bool IsPropaguleParent(const int relative_dir=0);

};
