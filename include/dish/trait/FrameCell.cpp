#pragma once
#ifndef DISHTINY_FRAMECELL_CPP_INCLUDE
#define DISHTINY_FRAMECELL_CPP_INCLUDE

#include <algorithm>
#include <list>
#include <memory>

#include "../../../third-party/Empirical/source/tools/Random.h"
#include "../../../third-party/Empirical/source/tools/random_utils.h"

#include "../config/Config.hpp"
#include "FrameCell.hpp"
#include "../topology/Cardi.hpp"
#include "../topology/GeometryHelper.hpp"

namespace dish {

FrameCell::FrameCell(
  emp::Random &local_rng_,
  const Config &cfg_,
  Manager &man_,
  const size_t pos_,
  const Config::inst_lib_t &inst_lib,
  const Config::inst_lib_t &inst_lib_spiker,
  const Config::event_lib_t &event_lib
) : man(man_)
  , local_rng(local_rng_)
  , cfg(cfg_)
  , neighs(GeometryHelper(cfg_).CalcLocalNeighs(pos_))
  , pos(pos_)
  , spiker(
    *this,
    std::numeric_limits<size_t>::max(),
    local_rng,
    cfg,
    inst_lib_spiker,
    event_lib
  ), connection_prune_count(0)
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
  shuffler = emp::vector<size_t>(
    [](){
      emp::vector<size_t> res(Cardi::Dir::NumDirs);
      std::iota(std::begin(res), std::end(res), 0);
      return res;
    }()
  );
}

FrameCell::~FrameCell() {
  for (auto & ptr : hw) ptr.Delete();
}

void FrameCell::Reset() {
  for(auto &fh : hw) fh->Reset();
  spiker.Reset();
  while (incoming_connections.size()) {
    RemoveIncomingConnection(*std::begin(incoming_connections));
  }
  connection_prune_count = 0;
  imprint.reset();
  daughter_imprint.reset();
}

void FrameCell::Process(const size_t update) {

  for(auto & fhw : hw) fhw->SetupCompute(update);
  spiker.SetupCompute(update);

  if (update % cfg.COMPUTE_FREQ()) return;

  emp_assert(shuffler.size() == Cardi::Dir::NumDirs);

  emp::Shuffle(local_rng, shuffler);

  for(size_t s = 0; s < cfg.HARDWARE_STEPS(); ++s) {
    for(size_t idx : shuffler) hw[idx]->StepProcess();
    if (cfg.RUN_SPIKER()) spiker.StepProcess();
  }

}

void FrameCell::SetProgram(const Config::program_t & program) {
  for(auto & fhw : hw) fhw->SetProgram(program);
}

void FrameCell::SetProgramSpiker(const Config::program_t & program) {
  spiker.SetProgram(program);
}

size_t FrameCell::GetPos() const {
  return pos;
}

size_t FrameCell::GetPauseSum(const size_t lev) const {

  size_t res = 0;
  for (size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
    res += man.Priority(neighs[d]).IsReprPaused(Cardi::Opp[d], lev);
  }

  return res;
}

Manager& FrameCell::Man() { return man; }

const Manager& FrameCell::Man() const { return man; }

FrameHardware& FrameCell::GetFrameHardware(const size_t dir) {
  return *hw[dir];
}

size_t FrameCell::GetNeigh(const size_t dir) const {
  return neighs[dir];
}

void FrameCell::QueueMessages(emp::vector<Config::inbox_t> &inboxes) {
  for (size_t i = 0; i < Cardi::Dir::NumDirs; ++i) {
    hw[i]->QueueExternalMessages(inboxes[i]);
  }
  emp_assert(inboxes[Cardi::Dir::NumDirs].size() == 0);
}

void FrameCell::QueueTrustedMessages(emp::vector<Config::inbox_t> &inboxes) {
  for (size_t i = 0; i < Cardi::Dir::NumDirs; ++i) {
    hw[i]->QueueInternalMessages(inboxes[i]);
  }
  spiker.QueueInternalMessages(inboxes[Cardi::Dir::NumDirs]);
}

emp::vector<Config::matchbin_t::state_t> FrameCell::CopyMatchBins() {
  emp::vector<Config::matchbin_t::state_t> res;
  res.reserve(Cardi::Dir::NumDirs);
  std::transform(
    std::begin(hw),
    std::end(hw),
    std::back_inserter(res),
    [](const auto & fhw){ return fhw->GetHardware().GetMatchBin().GetState(); }
  );
  return res;
}

void FrameCell::SetRegulators(
  const emp::vector<Config::matchbin_t::state_t> & target
) {
  for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
    hw[dir]->SetRegulators(target[dir]);
  }
}

FrameHardware& FrameCell::GetSpiker() { return spiker; }

void FrameCell::RegisterIncomingConnection(const size_t source) {
  incoming_connections.insert(source);
}


void FrameCell::DeleteIncomingConnection(const size_t source) {
  emp_assert(incoming_connections.size());
  emp_assert(incoming_connections.count(source));
  incoming_connections.erase(
    incoming_connections.find(source)
  );

}

void FrameCell::RemoveIncomingConnection(const size_t source) {
  auto & in_mutex = incoming_connection_mutex;
  auto & out_mutex = Man().Connection(source).outgoing_connection_mutex;

  // acquire both mutexes in one swoop to prevent deadlock
  std::lock(in_mutex, out_mutex);
  std::lock_guard<std::mutex> in_guard(in_mutex, std::adopt_lock);
  std::lock_guard<std::mutex> out_guard(out_mutex, std::adopt_lock);

  emp_assert(incoming_connections.size());
  emp_assert(incoming_connections.count(source));
  // call delete not remove to prevent infinite recursion
  Man().Connection(source).DeleteOutgoingConnection(pos);
  DeleteIncomingConnection(source);

}

void FrameCell::PruneIncomingConnection(const size_t connection_floor) {
  if (GetIncomingConectionCount() > connection_floor) {
    connection_prune_count = 1;
  }
}

void FrameCell::DoPrune() {
  while (incoming_connections.size() && connection_prune_count) {
    RemoveIncomingConnection(*std::begin(incoming_connections));
    --connection_prune_count;
  };
}

size_t FrameCell::GetIncomingConectionCount() const {
  return incoming_connections.size();
}

const std::unordered_multiset<size_t> &
FrameCell::ViewIncomingConnections() const {
  return incoming_connections;
}

void FrameCell::SetImprint(const std::optional<Config::tag_t> & imprint_) {
  imprint = imprint_;
}

void FrameCell::SetDaughterImprint(
  const std::optional<Config::tag_t> & daughter_imprint_
) { daughter_imprint = daughter_imprint_; }

const std::optional<Config::tag_t> & FrameCell::GetDaughterImprint() const {
  return daughter_imprint;
}

const std::optional<Config::tag_t> & FrameCell::GetImprint() const {
  return imprint;
}

} // namespace dish

#endif // #ifndef DISHTINY_FRAMECELL_CPP_INCLUDE
