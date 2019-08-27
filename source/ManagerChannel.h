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

  emp::vector<size_t> exp_noise;

  std::function<size_t()> age_getter;

public:

  ManagerChannel(
    const Config &cfg_,
    emp::Random &local_rng_,
    std::function<size_t()> age_getter_
  ) : ids(ChannelPack())
  , local_rng(local_rng_)
  , cfg(cfg_)
  , gen_counter(cfg_.NLEV())
  , cell_age_multipliers(cfg.NLEV(), 1.0)
  , age_getter(age_getter_)
  {
    for(size_t i = 0; i < cfg.NLEV(); ++i) {
      ids->push_back(drawChannelID());
      exp_noise.push_back(local_rng.GetUInt(cfg.GEN_INCR_FREQ()));
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
  size_t IsExpired(const size_t replev) const {

    size_t expired = 0;

    for(size_t i = replev; i < cfg.NLEV(); ++i) {

      const size_t lim = (
        (i+1) * cfg.Lev(i).EVENT_RADIUS() * cfg.AGE_LIMIT_MULTIPLIER()
      ) + (
        // add noise so cells don't tick over all at once
        age_getter() % cfg.GEN_INCR_FREQ() < exp_noise[i]
      );

      emp_assert(cell_age_multipliers[i] >= 1.0);

      if (GetGeneration(i) * cell_age_multipliers[i] > lim) {
        expired += GetGeneration(i) * cell_age_multipliers[i] - lim;
      }

    }

    return expired;
  }

  std::function<size_t(size_t)> MakeExpChecker() {
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

      // also generate a new exp noise
      exp_noise[i] = local_rng.GetUInt(cfg.GEN_INCR_FREQ());

    }


  }

};
