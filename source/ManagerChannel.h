#pragma once

#include <experimental/optional>

#include "tools/Random.h"

#include "ChannelPack.h"
#include "Config.h"

class ManagerChannel {

private:

  std::experimental::optional<ChannelPack> ids;

  emp::Random &local_rng;

  const Config &cfg;

  emp::vector<size_t> gen_counter;

  Config::chanid_t drawChannelID() {
    return local_rng.GetUInt64();
  }

public:

  ManagerChannel(
    const Config &cfg_,
    emp::Random &local_rng_
  ) : ids(ChannelPack())
  , local_rng(local_rng_)
  , cfg(cfg_)
  , gen_counter(cfg_.NLEV())
  {
    for(size_t i = 0; i < cfg.NLEV(); ++i) {
      ids->push_back(drawChannelID());
    }
  }

  const std::experimental::optional<Config::chanid_t> GetID(const size_t lev) const {
    return ids ? std::experimental::optional<Config::chanid_t>((*ids)[lev]) : std::experimental::nullopt;
  }

  std::experimental::optional<ChannelPack> GetIDs() const { return ids; }

  void ClearIDs() { ids = std::experimental::nullopt; }

  size_t GetGeneration(const size_t lev) const { return gen_counter[lev]; }

  void IncrCellAge(const size_t lev, const size_t amt) {
    gen_counter[lev] += amt;
  }

  const emp::vector<size_t>& GetGenCounter() const { return gen_counter; }

  // same replev used for Inherit
  bool IsExpired(const size_t replev) const {

    bool expired = false;

    for(size_t i = replev; i < cfg.NLEV(); ++i) {

      const size_t lim = cfg.Lev(i).EVENT_RADIUS() * 2;

      expired |= GetGeneration(i) > lim;
    }

    return expired;
  }

  bool CheckMatch(const ManagerChannel &other, const size_t lev) const {
    return GetID(lev) && other.GetID(lev) ? *GetID(lev) == *other.GetID(lev) : false;
  }

  // to be called on the parent
  void LogReprGen(const size_t replev) {
    for(size_t i = replev; i < cfg.NLEV(); ++i) gen_counter[i]++;
  }

  /* perform a certain level of reproduction */
  void Inherit(
    const ChannelPack &parent,
    const emp::vector<size_t> & parent_gen_counter,
    const size_t replev
  ) {

    if (!ids) ids = std::experimental::optional<ChannelPack>{ChannelPack(cfg.NLEV())};

    for(size_t i = 0; i < cfg.NLEV(); ++i) {
      if(i < replev) {
        (*ids)[i] = drawChannelID();
        gen_counter[i] = 0;
      } else {
        (*ids)[i] = parent[i];
        gen_counter[i] = parent_gen_counter[i] + 1;
      }
    }
  }

};
