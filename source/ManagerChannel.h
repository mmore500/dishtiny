#pragma once

#include "tools/Random.h"

#include "ChannelPack.h"
#include "Config.h"

class ManagerChannel {

private:

  ChannelPack ids;

  emp::Random &local_rng;

  Config::chanid_t drawChannelID() {
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

  const Config::chanid_t GetID(size_t lev) {
    return ids[lev];
  }

  ChannelPack GetIDs() {
    return ids;
  }

  bool CheckMatch(ManagerChannel &other, size_t lev) {
    return GetID(lev) == other.GetID(lev);
  }

  /* perform a certain level of reproduction */
  void Inherit(ChannelPack &parent, size_t lev) {
    for(size_t i = 0; i < ids.size(); ++i) {
      if(i < lev) {
        ids[i] = drawChannelID();
      } else {
        ids[i] = parent[i];
      }
    }
  }

};
