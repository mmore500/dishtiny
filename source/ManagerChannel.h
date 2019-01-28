#pragma once

#include "tools/Random.h"

#include "Config.h"

class ManagerChannel {

using chanid_t = uint64_t;

private:

  emp::vector<chanid_t> ids;

  emp::Random &local_rng;

  chanid_t drawChannelID() {
    return local_rng.GetUInt64();
  }

public:

  ManagerChannel(
    Config &cfg,
    emp::Random &local_rng_
  ) : local_rng(local_rng_)
  {
    for(size_t i = 0; i < cfg.NLEV(); ++i) {
      ids.push_back(drawChannelID());
    }
  }

  const chanid_t GetID(size_t lev) {
    return ids[lev];
  }

  bool CheckMatch( ManagerChannel &other, size_t lev) {
    return GetID(lev) == other.GetID(lev);
  }

  /* perform a certain level of reproduction */
  void Inherit(ManagerChannel &parent, size_t lev) {
    for(size_t i = 0; i < ids.size(); ++i) {
      if(i < lev) {
        ids[i] = drawChannelID();
      } else {
        ids[i] = parent.GetID(i);
      }
    }
  }

};
