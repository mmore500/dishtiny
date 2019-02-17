#pragma once

#include <algorithm>
#include <experimental/optional>
#include <tuple>

#include "base/Ptr.h"
#include "base/vector.h"
#include "tools/Random.h"
#include "tools/random_utils.h"

#include "ChannelPack.h"
#include "Genome.h"
#include "SirePack.h"

class ManagerPriority {

private:
  emp::Random &local_rng;
  emp::vector<size_t> priority_order;

  size_t cur_max_priority;
  emp::Ptr<Genome> gen;
  SirePack sp;

  bool TakesPriority(const size_t dir) {

    size_t i;

    for(i = 0; priority_order[i] != dir; ++i);

    const bool retval = (i <= cur_max_priority);

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
    // gen has been handed off to the population!
    // we can't delete, but can remove our reference here
    gen = nullptr;
  }

  void ProcessSire(
    SirePack sire_pack,
    Config::program_t &parent_program
  ){
    if (TakesPriority(sire_pack.dir)) {
      sp = sire_pack;
      if (gen) gen.Delete();
      gen.New(parent_program);
    }
  }

  std::experimental::optional<std::tuple<emp::Ptr<Genome>,SirePack>>
  QueryPendingGenome() const {
    return gen ? std::experimental::make_optional(std::make_tuple(gen,sp)) : std::experimental::nullopt;
  }

};
