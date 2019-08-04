#pragma once

#include <algorithm>
#include <list>

#include "tools/Random.h"
#include "tools/random_utils.h"

#include "Cardi.h"
#include "Config.h"
#include "GeometryHelper.h"
#include "FrameCell.h"

FrameCell::FrameCell(
  emp::Random &local_rng_,
  const Config &cfg_,
  Manager &man_,
  const size_t pos_,
  const Config::inst_lib_t &inst_lib,
  const Config::event_lib_t &event_lib
) : man(man_)
  , local_rng(local_rng_)
  , cfg(cfg_)
  , neighs(GeometryHelper(cfg_).CalcLocalNeighs(pos_))
  , pos(pos_)
{
  for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
    hw.push_back(emp::NewPtr<FrameHardware>(
      *this,
      dir,
      local_rng,
      cfg,
      inst_lib,
      event_lib
    ));
  }
}

FrameCell::~FrameCell() {
  for (auto & ptr : hw) ptr.Delete();
}

void FrameCell::Reset() { for(auto &fh : hw) fh->Reset(); }

void FrameCell::Process(const size_t update) {

  for(auto & fhw : hw) fhw->SetupCompute(update);

  static emp::vector<size_t> shuffler;
  if (!shuffler.size()) {
    for(size_t idx = 0; idx < hw.size(); ++idx) shuffler.push_back(idx);
  }
  emp::Shuffle(local_rng, shuffler);

  for(size_t s = 0; s < cfg.HARDWARE_STEPS(); ++s) {
    for(size_t idx : shuffler) hw[idx]->StepProcess();
  }

}

void FrameCell::SetProgram(const Config::program_t & program) {
  for(auto & fhw : hw) fhw->SetProgram(program);
}

size_t FrameCell::GetPos() const {
  return pos;
}

size_t FrameCell::GetPauseSum(const size_t lev) const {
  size_t res = 0;
  for (size_t i = 0; i < hw.size(); ++i) res += hw[i]->IsReprPaused(lev);
  return res;
}


Manager& FrameCell::Man() {
  return man;
}

FrameHardware& FrameCell::GetFrameHardware(const size_t dir) {
  return *hw[dir];
}

size_t FrameCell::GetNeigh(const size_t dir) const {
  return neighs[dir];
}

void FrameCell::QueueMessages(emp::vector<Config::inbox_t> &inboxes) {
  for(size_t i = 0; i < inboxes.size(); ++i) hw[i]->QueueMessages(inboxes[i]);
}
