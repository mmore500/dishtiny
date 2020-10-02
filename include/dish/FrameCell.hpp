#pragma once
#ifndef DISH_FRAMECELL_HPP_INCLUDE
#define DISH_FRAMECELL_HPP_INCLUDE

#include <mutex>
#include <algorithm>
#include <list>
#include <memory>

#include "../../third-party/Empirical/source/tools/Random.h"
#include "../../third-party/Empirical/source/tools/random_utils.h"

#include "Cardi.hpp"
#include "Config.hpp"
#include "GeometryHelper.hpp"
#include "FrameHardware.hpp"

namespace dish {

class FrameCell {

private:

  Manager &man;
  emp::Random &local_rng;
  const Config &cfg;

  emp::vector<emp::Ptr<FrameHardware>> hw;
  const emp::vector<size_t> neighs;
  const size_t pos;

  emp::vector<size_t> shuffler;

  std::unordered_multiset<size_t> incoming_connections;

  FrameHardware spiker;

  size_t connection_prune_count;

  std::optional<Config::tag_t> imprint;

  std::optional<Config::tag_t> daughter_imprint;

public:

  mutable std::mutex incoming_connection_mutex;

  FrameCell() = delete;

  FrameCell(
    emp::Random &local_rng_,
    const Config &cfg_,
    Manager &man_,
    const size_t pos_,
    const Config::inst_lib_t &inst_lib,
    const Config::inst_lib_t &inst_lib_spiker,
    const Config::event_lib_t &event_lib
  );

  ~FrameCell();

  void Reset();

  void Process(const size_t update);

  void SetProgram(const Config::program_t & program);

  void SetProgramSpiker(const Config::program_t & program);

  size_t GetPos() const;

  size_t GetPauseSum(const size_t lev) const;

  Manager& Man();

  const Manager& Man() const;

  FrameHardware& GetFrameHardware(const size_t dir);

  size_t GetNeigh(const size_t dir) const;

  void QueueMessages(emp::vector<Config::inbox_t> &inboxes);

  void QueueTrustedMessages(emp::vector<Config::inbox_t> &inboxes);

  emp::vector<Config::matchbin_t::state_t> CopyMatchBins();

  void SetRegulators(const emp::vector<Config::matchbin_t::state_t> & targets);

  void RegisterIncomingConnection(const size_t source);

  void DeleteIncomingConnection(const size_t source);

  void RemoveIncomingConnection(const size_t source);

  void PruneIncomingConnection(const size_t connection_floor=0);

  void DoPrune();

  FrameHardware& GetSpiker();

  size_t GetIncomingConectionCount() const;

  const std::unordered_multiset<size_t> & ViewIncomingConnections() const;

  void SetImprint(const std::optional<Config::tag_t> & imprint_);

  void SetDaughterImprint(
    const std::optional<Config::tag_t> & daughter_imprint_
  );

  const std::optional<Config::tag_t> & GetDaughterImprint() const;

  const std::optional<Config::tag_t> & GetImprint() const;

};

} // namespace dish

#endif // #ifndef DISH_FRAMECELL_HPP_INCLUDE
