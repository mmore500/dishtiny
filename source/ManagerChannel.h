#pragma once

#include "tools/Random.h"

class ManagerChannel {

using chanid_t = uint64_t;

private:

  emp::vector<chanid_t> ids;

  emp::Random &rng;

  chanid_t drawChannelID() {
    reutrn rng.GetUInt64();
  }

public:

  ChannelID(
    DishtinyConfig &config,
    emp::Random &rng_
  ) : rng(_rng)
  {
    for(size_t i = 0; i < config.NLEV(); ++i) {
      ids.push_back(drawChannelID());
    }
  }

  const chanid_t GetID(size_t lev) {
    return ids[lev];
  }

  const bool CheckMatch(const ManagerChannel &other, size_t lev) {
    return GetID(lev) == other.GetID(lev);
  }

  /* perform a certain level of reproduction */
  void Inherit(const ManagerChannel &parent, size_t lev) {
    for(size_t i = 0; i < *self.size(); ++i) {
      if(i < lev) {
        ids[i] = drawChannelID(rng);
      } else {
        ids[i] = parent[i];
      }
    }
  }

};
