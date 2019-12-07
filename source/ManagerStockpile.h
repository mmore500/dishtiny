#pragma once

#include <numeric>
#include <limits>

#include "base/assert.h"

#include "Config.h"

class ManagerStockpile {

private:

  const Config &cfg;

  emp::Random &local_rng;

  double resource;

  emp::vector<double> contrib_resource;
  double refund_resource;
  emp::vector<size_t> harvest_withdrawals;

  std::function<size_t(size_t)> expchecker;

public:

  ManagerStockpile(
    std::function<size_t(size_t)> expchecker_,
    emp::Random &local_rng_,
    const Config &cfg_
  ) : cfg(cfg_)
  , local_rng(local_rng_)
  , contrib_resource(Cardi::Dir::NumDirs+1) // +1 for spiker
  , refund_resource(0.0)
  , harvest_withdrawals(cfg_.NLEV())
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
    harvest_withdrawals[lev] += (amt <= 0.0);
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

  void Reset() {
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
    resource += cfg.BASE_RESOURCE_INFLOW();
  }

  void ApplyDecay(){ if (resource > 0) resource *= cfg.RESOURCE_DECAY(); }

  size_t QueryHarvestWithdrawals(size_t lev) const {
    return harvest_withdrawals[lev];
  }

  void ResetHarvestWithdrawals(size_t lev) {
    harvest_withdrawals[lev] = 0;
  }

};
