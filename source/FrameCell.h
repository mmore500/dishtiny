#pragma once

#include <algorithm>
#include <list>

#include "tools/Random.h"
#include "tools/random_utils.h"

#include "Cardi.h"
#include "Config.h"
#include "GeometryHelper.h"
#include "FrameHardware.h"

//forward declaration
class Manager;

class FrameCell {

private:

  Manager &man;
  emp::Random &local_rng;
  const Config &cfg;

  emp::vector<emp::Ptr<FrameHardware>> hw;
  const emp::vector<size_t> neighs;
  const size_t pos;

public:

  FrameCell(
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

  ~FrameCell() {
    for (auto & ptr : hw) ptr.Delete();
  }

  void Reset() { for(auto &fh : hw) fh->Reset(); }

  void Process() {

    for(auto & fhw : hw) fhw->SetupCompute();

    static emp::vector<size_t> shuffler;
    if (!shuffler.size()) {
      for(size_t idx = 0; idx < hw.size(); ++idx) shuffler.push_back(idx);
    }
    emp::Shuffle(local_rng, shuffler);

    for(size_t s = 0; s < cfg.HARDWARE_STEPS(); ++s) {
      for(size_t idx : shuffler) hw[idx]->StepProcess();
    }

  }

  void SetProgram(const Config::program_t & program) {
    for(auto & fhw : hw) fhw->SetProgram(program);
  }

  size_t GetPos() const {
    return pos;
  }

  Manager& Man() {
    return man;
  }

  FrameHardware& GetFrameHardware(const size_t dir) {
    return *hw[dir];
  }

  size_t GetNeigh(const size_t dir) const {
    return neighs[dir];
  }

  void QueueMessages(emp::vector<Config::inbox_t> &inboxes) {
    for(size_t i = 0; i < inboxes.size(); ++i) hw[i]->QueueMessages(inboxes[i]);
  }

};
