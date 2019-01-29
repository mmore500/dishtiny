#pragma once

#include <algorithm>
#include <optional>
#include <tuple>

#include "base/Ptr.h"
#include "base/vector.h"
#include "tools/Random.h"
#include "tools/random_utils.h"

#include "ChannelPack.h"

class Genome;

class ManagerPriority {

private:
  emp::Random &local_rng;
  emp::vector<size_t> priority_order;

  size_t cur_max_priority;
  emp::Ptr<Genome> g;
  ChannelPack cp;
  size_t par_pos;
  size_t rep_lev;

  bool TakesPriority(Cardi::Dir dir) {

    size_t i;

    for(i = 0; priority_order[i] != dir; ++i);

    bool retval = (i <= cur_max_priority);

    cur_max_priority = std::min(i,cur_max_priority);

    return retval;

  }

public:
  ManagerPriority(emp::Random &local_rng_)
  : local_rng(local_rng_)
  {
    for(size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
      priority_order.push_back(d);
    }
  }

  void Reset() {
    emp::Shuffle(local_rng, priority_order);
    cur_max_priority = Cardi::Dir::NumDirs;
    g = nullptr;
  }

  void ProcessSire(
    Cardi::Dir d,
    Genome &parent_genome,
    ChannelPack &parent_cp,
    size_t rep_lev_,
    size_t par_pos_
  ){
    if (TakesPriority(d)) {
      par_pos = par_pos_;
      rep_lev = rep_lev_;
      g = emp::NewPtr<Genome>(parent_genome);
      cp = parent_cp;
    }
  }

  std::optional<std::tuple<emp::Ptr<Genome>,ChannelPack,size_t,size_t>> QueryPendingGenome() {
    return g ? std::make_optional(std::make_tuple(g,cp,rep_lev,par_pos)) : std::nullopt;
  }

};
