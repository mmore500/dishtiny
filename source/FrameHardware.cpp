#pragma once

#include <algorithm>

#include "base/vector.h"
#include "base/Ptr.h"

#include "Config.h"
#include "FrameCell.h"
#include "Manager.h"
#include "DishWorld.h"
#include "FrameHardware.h"

FrameHardware::FrameHardware(
    FrameCell &cell_,
    const size_t facing_,
    emp::Random &local_rng_,
    const Config &cfg_,
    const Config::inst_lib_t &inst_lib,
    const Config::event_lib_t &event_lib
  ) : cell(cell_)
    , cfg(cfg_)
    , facing(facing_)
    , inbox_active(false)
    , stockpile_reserve(0)
    , cpu(inst_lib, event_lib, &local_rng_)
  {
    cpu.SetTrait(this);
  }

FrameCell& FrameHardware::Cell() { return cell; }

void FrameHardware::Reset() {
  inbox_active = false;
  stockpile_reserve = 0;

  cpu.ResetHardware();
  cpu.ResetProgram();
  emp_assert(!cpu.GetProgram().GetSize());
}

double FrameHardware::CheckStockpileReserve() const {
  return stockpile_reserve;
}

void FrameHardware::SetStockpileReserve(const double amt) {
  stockpile_reserve = std::max(0.0,amt);
}

void FrameHardware::DispatchEnvTriggers(){

  static emp::Random rng(1);
  static emp::vector<Config::tag_t> pro_trigger_tags;
  static emp::vector<Config::tag_t> anti_trigger_tags;

  size_t i = 0;

  // cell child trigger
  if(i >= pro_trigger_tags.size()) {
    pro_trigger_tags.emplace_back(rng);
    auto copy = pro_trigger_tags[i];
    anti_trigger_tags.emplace_back(copy.Toggle());
  }
  if (IsCellChild()) {
    cpu.TriggerEvent("EnvTrigger", pro_trigger_tags[i]);
  } else {
    // cpu.TriggerEvent("EnvTrigger", anti_trigger_tags[i]);
  }

  ++i;

  // cell parent trigger
  // if(i >= pro_trigger_tags.size()) {
  //   pro_trigger_tags.emplace_back(rng);
  //   auto copy = pro_trigger_tags[i];
  //   anti_trigger_tags.emplace_back(copy.Toggle());
  // }
  // if (IsCellParent()) {
  //   cpu.TriggerEvent("EnvTrigger", pro_trigger_tags[i]);
  // } else {
  //   // cpu.TriggerEvent("EnvTrigger", anti_trigger_tags[i]);
  // }
  //
  // ++i;

  // negative resource trigger
  if(i >= pro_trigger_tags.size()) {
    pro_trigger_tags.emplace_back(rng);
    auto copy = pro_trigger_tags[i];
    anti_trigger_tags.emplace_back(copy.Toggle());
  }
  if (Cell().Man().Stockpile(Cell().GetPos()).QueryResource() < 0) {
    cpu.TriggerEvent("EnvTrigger", pro_trigger_tags[i]);
  } else {
    // cpu.TriggerEvent("EnvTrigger", anti_trigger_tags[i]);
  }

  ++i;

  // harvest withdrawals triggers
  for (size_t lev = 0; lev < cfg.NLEV(); ++lev) {

    if (i >= pro_trigger_tags.size()) {
      pro_trigger_tags.emplace_back(rng);
      auto copy = pro_trigger_tags[i];
      anti_trigger_tags.emplace_back(copy.Toggle());
    }

//  if (!Cell().Man().Stockpile(Cell().GetPos()).QueryHarvestWithdrawals(lev)) {
//    cpu.TriggerEvent("EnvTrigger", anti_trigger_tags[i]);
//  }
    for (
      size_t w = 0;
      w < Cell().Man().Stockpile(Cell().GetPos()).QueryHarvestWithdrawals(lev);
      ++w
    ) {
      if (Cell().Man().Stockpile(Cell().GetPos()).QueryResource() < 0) {
        cpu.TriggerEvent("EnvTrigger", pro_trigger_tags[i]);
      }
    }

    Cell().Man().Stockpile(Cell().GetPos()).ResetHarvestWithdrawals(lev);

    ++i;

  }

  // channel match triggers
  for(size_t lev = 0; lev < cfg.NLEV(); ++lev) {
    if(i >= pro_trigger_tags.size()) {
      pro_trigger_tags.emplace_back(rng);
      auto copy = pro_trigger_tags[i];
      anti_trigger_tags.emplace_back(copy.Toggle());
    }
    if (cfg.CHANNELS_VISIBLE()) {
      if (IsChannelMate(lev)) {
        cpu.TriggerEvent("EnvTrigger", pro_trigger_tags[i]);
      } else {
        // cpu.TriggerEvent("EnvTrigger", anti_trigger_tags[i]);
      }
    }
    ++i;
  }

  // propagule child trigger
  if(i >= pro_trigger_tags.size()) {
    pro_trigger_tags.emplace_back(rng);
    auto copy = pro_trigger_tags[i];
    anti_trigger_tags.emplace_back(copy.Toggle());
  }
  if (cfg.CHANNELS_VISIBLE()) {
    if (IsPropaguleChild()) {
      cpu.TriggerEvent("EnvTrigger", pro_trigger_tags[i]);
    } else {
      // cpu.TriggerEvent("EnvTrigger", anti_trigger_tags[i]);
    }
  }

  ++i;

  // propagule parent trigger
  // if(i >= pro_trigger_tags.size()) {
  //   pro_trigger_tags.emplace_back(rng);
  //   auto copy = pro_trigger_tags[i];
  //   anti_trigger_tags.emplace_back(copy.Toggle());
  // }
  // if (cfg.CHANNELS_VISIBLE()) {
  //   if (IsPropaguleParent()) {
  //     cpu.TriggerEvent("EnvTrigger", pro_trigger_tags[i]);
  //   } else {
  //     cpu.TriggerEvent("EnvTrigger", anti_trigger_tags[i]);
  //   }
  // }

  //++i;

  // update trigger
  if(i >= pro_trigger_tags.size()) {
    pro_trigger_tags.emplace_back(rng);
    auto copy = pro_trigger_tags[i];
    anti_trigger_tags.emplace_back(copy.Toggle());
  }
  cpu.TriggerEvent("EnvTrigger", pro_trigger_tags[i]);

}

void FrameHardware::SetupCompute(const size_t update) {
  if (update % cfg.ENV_TRIG_FREQ() == 0) DispatchEnvTriggers();
}

void FrameHardware::StepProcess() {
  emp_assert(cpu.GetProgram().GetSize());
  cpu.SingleProcess();
}

void FrameHardware::SetProgram(const Config::program_t & program) {
  cpu.SetProgram(program);
}

size_t FrameHardware::GetFacing() const {
  return facing;
}

size_t FrameHardware::GetMsgDir() const {
  return msg_dir;
}

void FrameHardware::SetMsgDir(const size_t new_dir) {
  msg_dir = new_dir;
}

void FrameHardware::SetInboxActivity(const bool state) {
  inbox_active = state;
}

bool FrameHardware::CheckInboxActivity() const {
  return inbox_active;
}

void FrameHardware::QueueMessage(const Config::event_t &event) {
  cpu.QueueEvent(event);
}

void FrameHardware::QueueMessages(Config::inbox_t &inbox) {
  while(inbox_active && !inbox.empty()) {
    QueueMessage(inbox.front());
    inbox.pop_front();
  }
  // clear inactive inboxes, too!
  inbox.clear();
}

size_t FrameHardware::CalcDir(const double relative_dir) {
  return emp::Mod(
    static_cast<size_t>(relative_dir) + GetFacing(),
    Cardi::NumDirs
  );
}

bool FrameHardware::IsLive(const int relative_dir/*=0*/) {
  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  return Cell().Man().DW().IsOccupied(neigh);
}

bool FrameHardware::IsOccupied(const int relative_dir/*=0*/) {
  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  return (bool) Cell().Man().Channel(neigh).GetIDs();
}

bool FrameHardware::IsCellChild(const int relative_dir/*=0*/) {

  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  Manager &man = Cell().Man();
  const size_t pos = Cell().GetPos();

  return (
    IsLive(relative_dir)
    && man.Family(pos).HasChildPos(neigh)
    && man.Family(neigh).IsParentPos(pos)
  );

}

bool FrameHardware::IsCellParent(const int relative_dir/*=0*/) {

  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  Manager &man = Cell().Man();
  const size_t pos = Cell().GetPos();

  return (
    IsLive(relative_dir)
    && man.Family(pos).IsParentPos(neigh)
    && man.Family(neigh).HasChildPos(pos)
  );

}

bool FrameHardware::IsChannelMate(const size_t lev, const int relative_dir/*=0*/) {

  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  Manager &man = Cell().Man();
  const size_t pos = Cell().GetPos();

  // should be able to sense partial apoptosis tiles
  return man.Channel(pos).CheckMatch(man.Channel(neigh), lev);

}

bool FrameHardware::IsPropaguleChild(const int relative_dir/*=0*/) {

  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  Manager &man = Cell().Man();
  const size_t pos = Cell().GetPos();

  return (
    IsLive(relative_dir)
    && (
      man.Family(neigh).GetPrevChan() == *man.Channel(pos).GetID(cfg.NLEV()-1)
    ));

}

bool FrameHardware::IsPropaguleParent(const int relative_dir/*=0*/) {

  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  Manager &man = Cell().Man();
  const size_t pos = Cell().GetPos();

  return (
    IsLive(relative_dir)
    && (
      man.Family(pos).GetPrevChan() == *man.Channel(neigh).GetID(cfg.NLEV()-1)
    ));

}

const Config::hardware_t& FrameHardware::GetHardware() {
  return cpu;
}

void FrameHardware::SetRegulators(Config::matchbin_t & target_mb) {

  cpu.GetMatchBin().ImprintRegulators(target_mb);

}
