#pragma once

#include <algorithm>
#include <experimental/optional>
#include <tuple>

#include "base/Ptr.h"
#include "base/vector.h"
#include "tools/Random.h"
#include "tools/random_utils.h"

#include "ChannelPack.h"
#include "Config.h"
#include "Genome.h"
#include "SirePack.h"

class ManagerPriority {

private:
  const Config &cfg;

  emp::Random &local_rng;
  emp::vector<size_t> priority_order;

  size_t cur_max_priority;
  emp::Ptr<Genome> gen;
  SirePack sp;

  emp::vector<emp::vector<size_t>> replev_dir_count;

  bool TakesPriority(const size_t dir) {

    size_t i;

    for(i = 0; priority_order[i] != dir; ++i);

    const bool retval = (i <= cur_max_priority);

    cur_max_priority = std::min(i,cur_max_priority);

    return retval;

  }

public:
  ManagerPriority(const Config &cfg_, emp::Random &local_rng_)
  : cfg(cfg_)
  , local_rng(local_rng_)
  {
    for(size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
      priority_order.push_back(d);
      replev_dir_count.push_back(emp::vector<size_t>(cfg.NLEV()+1));
    }
  }

  void Reset() {
    emp::Shuffle(local_rng, priority_order);
    cur_max_priority = Cardi::Dir::NumDirs;
    // gen has been handed off to the population!
    // we can't delete, but can remove our reference here
    gen = nullptr;
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      for(size_t replev = 0; replev < cfg.NLEV()+1; ++replev) {
        replev_dir_count[dir][replev] = 0;
      }
    }
  }

  void ProcessSire(
    const SirePack sire_pack,
    Config::program_t &parent_program
  ){

    replev_dir_count[Cardi::Opp[sire_pack.outgoing_dir]][sire_pack.replev]++;

    if (TakesPriority(sire_pack.outgoing_dir)) {
      sp = sire_pack;
      if (gen) gen.Delete();
      gen.New(parent_program);
    }
  }

  size_t GetCount(const size_t incoming_dir, const size_t replev) const {
    return replev_dir_count[incoming_dir][replev];
  }

  std::experimental::optional<std::tuple<emp::Ptr<Genome>,SirePack>>
  QueryPendingGenome() const {
    return gen ? std::experimental::make_optional(std::make_tuple(gen,sp)) : std::experimental::nullopt;
  }

};
