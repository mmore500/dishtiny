#pragma once

#include <algorithm>

#include "base/vector.h"
#include "base/Ptr.h"

#include "Config.h"
#include "FrameCell.h"
#include "Manager.h"
#include "DishWorld.h"
#include "FrameHardware.h"
#include "Genome.h"

FrameHardware::FrameHardware(
    FrameCell &cell_,
    const size_t facing_,
    emp::Random &local_rng_,
    const Config &cfg_,
    const Config::inst_lib_t &inst_lib,
    const Config::event_lib_t &event_lib
  ) : cell(cell_)
    , local_rng(local_rng_)
    , cfg(cfg_)
    , facing(facing_)
    , inbox_active(true)
    , stockpile_reserve(0.0)
    , stockpile_reserve_fresh(0)
    , reproduction_reserve(0.0)
    , reproduction_reserve_fresh(0)
    , cpu(inst_lib, event_lib, &local_rng_)
    , external_membrane(local_rng_)
    , internal_membrane(local_rng_)
  { cpu.SetTrait(this); }

FrameCell& FrameHardware::Cell() { return cell; }

void FrameHardware::Reset() {
  inbox_active = true;

  stockpile_reserve = 0.0;
  stockpile_reserve_fresh = 0;

  reproduction_reserve = 0.0;
  reproduction_reserve_fresh = 0;

  cpu.ResetHardware();
  cpu.ResetProgram();
  emp_assert(!cpu.GetProgram().GetSize());

  external_membrane.Reset();
  internal_membrane.Reset();
}

double FrameHardware::CheckStockpileReserve() const {
  return stockpile_reserve;
}

void FrameHardware::SetStockpileReserve(const double amt, const size_t dur) {
  stockpile_reserve = std::max(0.0,amt);
  stockpile_reserve_fresh = dur;
}

void FrameHardware::TryClearStockpileReserve() {
  if (stockpile_reserve_fresh) --stockpile_reserve_fresh;
  if (!stockpile_reserve_fresh) stockpile_reserve = 0.0;
}

double FrameHardware::CheckReproductionReserve() const {
  return reproduction_reserve;
}

void FrameHardware::SetReproductionReserve(const double amt, const size_t dur) {
  reproduction_reserve = std::max(0.0,amt);
  reproduction_reserve_fresh = dur;
}

void FrameHardware::TryClearReproductionReserve() {
  if (reproduction_reserve_fresh) --reproduction_reserve_fresh;
  if (!reproduction_reserve_fresh) reproduction_reserve = 0.0;
}

void FrameHardware::DispatchEnvTriggers(const size_t update){

  // need at least 12 cpus available
  emp_assert(cfg.HW_MAX_CORES() > 10);
  // ... so make sure at least 10 are unoccupied
  cpu.SetMaxCores(cfg.HW_MAX_CORES() - 10);
  cpu.SetMaxCores(cfg.HW_MAX_CORES());


  Genome & g = Cell().Man().DW().GetOrg(Cell().GetPos());

  // update trigger
  cpu.TriggerEvent("EnvTrigger", g.GetTag(0));

  // was just born trigger
  if (Cell().Man().Family(Cell().GetPos()).GetCellAge(update) == 0) {
    cpu.TriggerEvent("EnvTrigger", g.GetTag(1));
  } else {
  // cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
  }

  if (facing < Cardi::Dir::NumDirs) { // not spiker
  // neighbor has more resource or less?
  if (IsPoorerThan() && IsLive()) {
    cpu.TriggerEvent("EnvTrigger", g.GetTag(3));
  } else if (IsLive()) {
    cpu.TriggerEvent("EnvTrigger", g.GetTag(4));
  }
  }

  // neighbor is dead or live?
  // if (IsLive()) {
  //   cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
  // } else {
  //   cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
  // }

  // neighbor cell age older or younger?
  // if (IsOlderThan() && IsLive()) {
  //   cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
  // } else if (IsLive()) {
  //   cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
  // }

  // stochastic trigger
  // if (local_rng.GetDouble() < cfg.STOCHASTIC_TRIGGER_FREQ()) {
  //   cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
  // } else {
  // // cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
  // }
  if (cfg.CHANNELS_VISIBLE() && facing < Cardi::Dir::NumDirs) { // not spiker

    // is neighbor expired?
    // if (IsExpired(0)) {
    //   cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
    // } else {
    //   // cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
    // }

    // cell parent trigger
    // note: mutually exclusive with cell child trigger
    if (IsCellParent()) {
      cpu.TriggerEvent("EnvTrigger", g.GetTag(5));
    } else {
      // cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
    }

    // cell child trigger
    if (IsCellChild()) {
      cpu.TriggerEvent("EnvTrigger", g.GetTag(6));
    } else {
      // cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
    }

    // propagule child trigger
    if (IsPropaguleChild()) {
      cpu.TriggerEvent("EnvTrigger", g.GetTag(7));
    } else {
      // cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
    }

    // propagule parent trigger
    // note: mutually exclusive with propagule child trigger
    if (IsPropaguleParent()) {
      cpu.TriggerEvent("EnvTrigger", g.GetTag(8));
    } else {
      // cpu.TriggerEvent("EnvTrigger", g.GetTag(X));
    }

  }

  for(size_t lev = 0; lev < cfg.NLEV(); ++lev) {

    if (cfg.CHANNELS_VISIBLE()) {
      const size_t exp = Cell().Man().Channel(Cell().GetPos()).IsExpired(lev);
      // self-expiration trigger
      if (!exp) {
        // not expired at all
        cpu.TriggerEvent("EnvTrigger", g.GetTag(9+6*lev));
      } else if (exp <= cfg.EXP_GRACE_PERIOD()) {
        // slightly exprired
        cpu.TriggerEvent("EnvTrigger", g.GetTag(10+6*lev));
      } else {
        // totally expired
        cpu.TriggerEvent("EnvTrigger", g.GetTag(11+6*lev));
      }

      if (facing < Cardi::Dir::NumDirs) { // not spiker
      // channel match triggers
      if (IsChannelMate(lev)) {
        cpu.TriggerEvent("EnvTrigger", g.GetTag(12+6*lev));
      } else {
        cpu.TriggerEvent("EnvTrigger", g.GetTag(13+6*lev));
      }
      }
    }

    // harvest withdrawal trigger
    if (Cell().Man().Stockpile(Cell().GetPos()).QueryHarvestWithdrawals(lev)) {
      cpu.TriggerEvent(
        "EnvTrigger",
        g.GetTag(14+6*lev)
      );
    }
    // clean up
    Cell().Man().Stockpile(Cell().GetPos()).ResetHarvestWithdrawals(lev);


  }

}

void FrameHardware::SetupCompute(const size_t update) {
  if (update % cfg.ENV_TRIG_FREQ() == 0) {

    cpu.GetMatchBin().DecayRegulators();
    external_membrane.Decay();
    internal_membrane.Decay();

    DispatchEnvTriggers(update);

    TryClearReproductionReserve();

    TryClearStockpileReserve();

  }
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

void FrameHardware::QueueInternalMessage(const Config::event_t &event) {
  if (GetInternalMembrane().LookupLaxAllOf(event.affinity)) QueueMessage(event);
}

void FrameHardware::QueueInternalMessages(Config::inbox_t &inbox) {
  while(!inbox.empty()) {
    QueueInternalMessage(inbox.front());
    inbox.pop_front();
  }
}

void FrameHardware::QueueMessage(const Config::event_t &event) {
  cpu.QueueEvent(event);
}

void FrameHardware::QueueExternalMessages(Config::inbox_t &inbox) {
  while(!inbox.empty()) {
    QueueExternalMessage(inbox.front());
    inbox.pop_front();
  }
}

void FrameHardware::QueueExternalMessage(const Config::event_t &event) {
  if (GetExternalMembrane().LookupStrictAllOf(event.affinity)) {
    QueueMessage(event);
  }
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

double FrameHardware::IsPoorerThan(const int relative_dir/*=0*/) {

  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  Manager &man = Cell().Man();
  const size_t pos = Cell().GetPos();

  return std::max(
    man.Stockpile(neigh).QueryResource() - man.Stockpile(pos).QueryResource(),
    0.0
  );

}

bool FrameHardware::IsOlderThan(const int relative_dir/*=0*/) {

  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  Manager &man = Cell().Man();
  const size_t pos = Cell().GetPos();

  return (
    man.Family(pos).GetBirthUpdate() <= man.Family(neigh).GetBirthUpdate()
  );

}

size_t FrameHardware::IsExpired(
  const size_t lev,
  const int relative_dir/*=0*/
) {

  const size_t dir = CalcDir(relative_dir);
  const size_t neigh = Cell().GetNeigh(dir);
  Manager &man = Cell().Man();

  return man.Channel(neigh).IsExpired(lev);

}

const Config::hardware_t& FrameHardware::GetHardware() {
  return cpu;
}

void FrameHardware::SetRegulators(Config::matchbin_t & target_mb) {

  cpu.GetMatchBin().ImprintRegulators(target_mb);

}

void FrameHardware::SetMatchBinState(
  const Config::matchbin_t::state_t & state
) {

  cpu.GetMatchBin().SetState(state);

}

Membrane & FrameHardware::GetExternalMembrane() {
  return external_membrane;
}


Membrane & FrameHardware::GetInternalMembrane() {
  return internal_membrane;
}
