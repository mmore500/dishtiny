#pragma once

#include <algorithm>

#include "tools/Random.h"
#include "tools/random_utils.h"

class ManagerPriority {

private:
  emp::Random &local_rng;
  emp::vector<size_t> priority_order;
  size_t cur_max_priority;

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
  }

  bool TakesPriority(Cardi::Dir dir) {

    size_t i;

    for(i = 0; priority_order[i] != dir; ++i);

    bool retval = (i <= cur_max_priority);

    cur_max_priority = std::min(i,cur_max_priority);

    return retval;

  }



};
