#pragma once

#include <optional>

#include "tools/Random.h"

#include "ChannelPack.h"
#include "Config.h"

class ManagerChannel {

private:

  std::optional<ChannelPack> ids;

  emp::Random &local_rng;

  const Config &cfg;

  emp::vector<size_t> gen_counter;

  emp::vector<double> cell_age_multipliers;

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
  , cell_age_multipliers(cfg.NLEV(), 1.0)
  {
    for(size_t i = 0; i < cfg.NLEV(); ++i) {
      ids->push_back(drawChannelID());
    }
  }

  void IncrGenCounts() { for (auto & g : gen_counter) ++g; }

  const std::optional<Config::chanid_t> GetID(const size_t lev) const {
    return ids ? std::optional<Config::chanid_t>((*ids)[lev]) : std::nullopt;
  }

  std::optional<ChannelPack> GetIDs() const { return ids; }

  void ClearIDs() {
    ids = std::nullopt;
    std::fill(
      std::begin(cell_age_multipliers),
      std::end(cell_age_multipliers),
      1.0
    );
  }

  size_t GetGeneration(const size_t lev) const { return gen_counter[lev]; }

  void SetCellAgeMultiplier(const size_t lev, const double amt) {
    emp_assert(amt >= 1.0);
    cell_age_multipliers[lev] = amt;
  }

  const emp::vector<size_t>& GetGenCounter() const { return gen_counter; }

  // same replev used for Inherit
  bool IsExpired(const size_t replev) const {

    bool expired = false;

    for(size_t i = replev; i < cfg.NLEV(); ++i) {

      const size_t lim = cfg.Lev(i).EVENT_RADIUS() * cfg.AGE_LIMIT_MULTIPLIER();

      emp_assert(cell_age_multipliers[i] >= 1.0);

      expired |= GetGeneration(i) * cell_age_multipliers[i] > lim;
    }

    return expired;
  }

  std::function<bool(size_t)> MakeExpChecker() {
    return [&](const size_t replev){ return IsExpired(replev); };
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

    std::fill(
      std::begin(cell_age_multipliers),
      std::end(cell_age_multipliers),
      1.0
    );

    if (!ids) ids = std::optional<ChannelPack>{ChannelPack(cfg.NLEV())};

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
