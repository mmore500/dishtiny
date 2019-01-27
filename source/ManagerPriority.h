#pragma once

#include <algorithm>

#include "tools/Random.h"
#include "tools/random_utils.h"

class ManagerPriority {

private:
  emp::Random &rng;
  emp::vector<Cardi::Dir> priority_order;
  size_t cur_max_priority;

public:
  ManagerPriority(emp::Random &rng_)
  : rng(rng_)
  , last_shuffled(-1)
  {
    for(Cardi::Dir d = 0; d < Cardi::Dir::NumDirs; ++d) {
      priority_order.push_back(d);
    }
  }

  void Reset() {
    emp::Shuffle(priority_order, rng);
    cur_max_priority = Cardi::Dir::NumDirs;
  }

  bool TakesPriority(Cardi::Dir dir) {

    size_t i;

    for(i = 0; priority_order[i] != dir; ++i);

    bool retval = (i <= cur_max_priority);

    cur_max_priority = std::min(i,cur_max_priority);

    return retval;

  }



};
