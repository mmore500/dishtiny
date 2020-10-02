#pragma once
#ifndef DISH_MANAGERCHANNEL_HPP_INCLUDE
#define DISH_MANAGERCHANNEL_HPP_INCLUDE

#include <optional>

#include "../../third-party/Empirical/source/tools/Random.h"

#include "ChannelPack.hpp"
#include "Config.hpp"

namespace dish {

class ManagerChannel {

private:

  std::optional<ChannelPack> ids;

  emp::Random &local_rng;

  const Config &cfg;

  emp::vector<size_t> gen_counter;

  emp::vector<double> cell_age_boosters;
  emp::vector<size_t> cell_age_boosters_fresh;

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
  , gen_counter(cfg.NLEV(), 0)
  , cell_age_boosters(cfg.NLEV(), 0.0)
  , cell_age_boosters_fresh(cfg.NLEV(), 0)
  , age_getter(age_getter_)
  {
    for(size_t i = 0; i < cfg.NLEV(); ++i) {
      ids->push_back(drawChannelID());
      exp_noise.push_back(local_rng.GetUInt(cfg.GEN_INCR_FREQ()));
    }
  }

  void IncrGenCounts() { for (auto & g : gen_counter) if (g+1) ++g; }

  const std::optional<Config::chanid_t> GetID(const size_t lev) const {
    return ids ? std::optional<Config::chanid_t>((*ids)[lev]) : std::nullopt;
  }

  std::optional<ChannelPack> GetIDs() const { return ids; }

  void ClearIDs() {
    ids = std::nullopt;
    std::fill(
      std::begin(cell_age_boosters),
      std::end(cell_age_boosters),
      0.0
    );
    std::fill(
      std::begin(gen_counter),
      std::end(gen_counter),
      0
    );
    std::fill(
      std::begin(cell_age_boosters_fresh),
      std::end(cell_age_boosters_fresh),
      0
    );
  }

  size_t GetGeneration(const size_t lev) const { return gen_counter[lev]; }

  void SetCellAgeBooster(const size_t lev, const size_t duration) {
    cell_age_boosters[lev] = std::max(
      GetGeneration(lev) - CalcExpLim(lev),
      0.0
    );

    cell_age_boosters_fresh[lev] = duration;

  }

  void DecayCellAgeBoosters() {
    for (size_t lev = 0; lev < cfg.NLEV(); ++lev) {
      if (cell_age_boosters_fresh[lev]) {
        --cell_age_boosters_fresh[lev];
      }
      if (!cell_age_boosters_fresh[lev]) {
        cell_age_boosters[lev] = 0.0;
      }
    }
  }

  const emp::vector<size_t>& GetGenCounter() const { return gen_counter; }

  double CalcExpLim(const size_t lev) const {
    return (
      cfg.Lev(lev).EVENT_RADIUS() * cfg.AGE_LIMIT_MULTIPLIER()
    ) + (
     // add noise so cells don't tick over all at once
     ((age_getter() % cfg.GEN_INCR_FREQ()) < exp_noise[lev]) ? 1.0 : 0.0
    );
  }

  // same replev used for Inherit
  double IsExpired(const size_t replev) const {

    double expired = 0.0;

    for (size_t i = replev; i < cfg.NLEV(); ++i) {

      const double lim = CalcExpLim(i);

      emp_assert(cell_age_boosters[i] >= 0.0);

      if (
        static_cast<double>(GetGeneration(i)) + cell_age_boosters[i] > lim
      ) {
        expired += (
          static_cast<double>(GetGeneration(i)) + cell_age_boosters[i] - lim
        );
      }

    }

    return expired;
  }

  emp::vector<double> GetExpirations() {
    emp::vector<double> res;
    for (size_t i = 0; i < cfg.NLEV() + 1; ++i) {
      res.push_back(IsExpired(i));
    }
    return res;
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
      std::begin(cell_age_boosters),
      std::end(cell_age_boosters),
      0.0
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

} // namespace dish

#endif // #ifndef DISH_MANAGERCHANNEL_HPP_INCLUDE
