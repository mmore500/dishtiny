#pragma once

#include <numeric>
#include <limits>

#include "base/assert.h"

#include "Config.h"

class ManagerStockpile {

private:

  const Config &cfg;

  double resource;

  emp::vector<double> contrib_resource;
  double refund_resource;
  emp::vector<size_t> harvest_withdrawals;

  emp::vector<double> in_resistance;
  emp::vector<size_t> in_resistance_fresh;

  emp::vector<double> out_resistance;
  emp::vector<size_t> out_resistance_fresh;

  emp::vector<std::function<void()>> sharing_doers;

  std::function<size_t(size_t)> expchecker;

public:

  ManagerStockpile(
    std::function<size_t(size_t)> expchecker_,
    const Config &cfg_
  ) : cfg(cfg_)
  , contrib_resource(Cardi::Dir::NumDirs)
  , refund_resource(0.0)
  , harvest_withdrawals(cfg_.NLEV())
  , in_resistance(Cardi::Dir::NumDirs)
  , in_resistance_fresh(Cardi::Dir::NumDirs)
  , out_resistance(Cardi::Dir::NumDirs)
  , out_resistance_fresh(Cardi::Dir::NumDirs)
  , expchecker(expchecker_)
  { Reset(); }

  double QueryResource() const {
    return resource;
  }

  double RequestResourceAmt(const double amt) {
    emp_assert(amt >= 0);
    if (resource > 0) {
      double actual = std::min(resource, amt);
      resource -= actual;
      return actual;
    } else {
      return 0;
    }
  }

  double RequestResourceFrac(const double frac, const double reserve) {
    emp_assert(frac >= 0 && frac <= 1);
    emp_assert(reserve >= 0);
    if (resource - reserve > 0) {
      double amt = (resource - reserve) * frac;
      resource -= amt;
      return amt;
    } else {
      return 0;
    }
  }

  void InternalApplyHarvest(const size_t lev, const double amt) {
    // if ANY level expired, can't collect resource
    const size_t exp = expchecker(0);
    harvest_withdrawals[lev] += amt < 0.0;
    if (exp <= cfg.EXP_GRACE_PERIOD()) {
      // grace period to allow a few reproductions without sharing
      resource += amt;
    } else {
      // no reward, maybe penalty
      resource += std::min(0.0, amt);
    }

  }

  void InternalAcceptResource(const double amt) {
    emp_assert(amt >= 0);
    resource += amt;
  }

  std::function<void()> MakeRepRefunder() {
    return [&](){ refund_resource += cfg.REP_THRESH(); };
  }

  void ExternalContribute(const double amt, const size_t incoming_dir) {
    emp_assert(amt >= 0);
    contrib_resource[incoming_dir] += amt;
  }

  double QueryExternalContribute(const size_t incoming_dir) const {
    return contrib_resource[incoming_dir];
  }

  double QueryTotalContribute() const {
    return std::accumulate(
      contrib_resource.begin(),contrib_resource.end(), 0.0
    );
  }

  void ResolveExternalContributions() {
    // shared resource was sitting around for an update, so it decayed
    resource += refund_resource * cfg.RESOURCE_DECAY();
    refund_resource = 0.0;
    for(size_t dir = 0; dir < contrib_resource.size(); ++dir) {
      // shared resource was sitting around for an update, so it decayed
      resource += contrib_resource[dir] * cfg.RESOURCE_DECAY();
      contrib_resource[dir] = 0.0;
    }
  }

  bool IsBankrupt() const {
    return cfg.KILL_THRESH() >= resource;
  }

  bool CheckInResistance(const size_t outgoing_dir) const {
    return in_resistance[outgoing_dir];
  }

  bool CheckOutResistance(const size_t outgoing_dir) const {
    return out_resistance[outgoing_dir];
  }

  void SetInResistance(
    const size_t outgoing_dir,
    const double set,
    const size_t dur
  ) {
    emp_assert(set >= 0.0);
    emp_assert(set <= 1.0);
    in_resistance[outgoing_dir] = set;
    in_resistance_fresh[outgoing_dir] = dur;
  }

  void SetOutResistance(
    const size_t outgoing_dir,
    const double set,
    const size_t dur
  ) {
    emp_assert(set >= 0.0);
    emp_assert(set <= 1.0);
    out_resistance[outgoing_dir] = set;
    out_resistance_fresh[outgoing_dir] = dur;
  }


  void ResolveNextResistance() {
    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {

      if (out_resistance_fresh[dir]) --out_resistance_fresh[dir];
      if (in_resistance_fresh[dir]) --in_resistance_fresh[dir];

      if (!out_resistance_fresh[dir]) out_resistance[dir] = 0.0;
      if (!in_resistance_fresh[dir]) in_resistance[dir] = 0.0;

    }
  }

  void AddSharingDoer(std::function<void()> doer) {
    sharing_doers.push_back(doer);
  }

  void CleanSharingDoers(const size_t update) {
    if (update % cfg.ENV_TRIG_FREQ()) {
      for (auto & doer : sharing_doers) doer();
    } else {
      sharing_doers.clear();
    }

  }

  void ProcessSharingDoers(const size_t update) {
    if (update % cfg.ENV_TRIG_FREQ() == 0) sharing_doers.clear();
  }

  void Reset() {
    sharing_doers.clear();
    std::fill(std::begin(in_resistance), std::end(in_resistance), 0.0);
    std::fill(
      std::begin(in_resistance_fresh),
      std::end(in_resistance_fresh),
      0
    );
    std::fill(std::begin(out_resistance), std::end(out_resistance), 0.0);
    std::fill(
      std::begin(out_resistance_fresh),
      std::end(out_resistance_fresh),
      0
    );
    resource = 0.0;
    std::fill(std::begin(contrib_resource), std::end(contrib_resource), 0.0);
    refund_resource = 0.0;
    std::fill(
      std::begin(harvest_withdrawals),
      std::end(harvest_withdrawals),
      0
    );
  }

  void ApplyBaseInflow() {
    for (size_t lev = 0; lev < cfg.NLEV(); ++lev) {
      InternalApplyHarvest(
        lev,
        cfg.BASE_RESOURCE_INFLOW()/static_cast<double>(cfg.NLEV())
      );
    }
  }

  void ApplyDecay(){ if (resource > 0) resource *= cfg.RESOURCE_DECAY(); }

  size_t QueryHarvestWithdrawals(size_t lev) const {
    return harvest_withdrawals[lev];
  }

  void ResetHarvestWithdrawals(size_t lev) {
    harvest_withdrawals[lev] = 0;
  }

};
