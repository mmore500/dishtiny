#pragma once

#include <numeric>

#include "base/assert.h"

#include "Config.h"

class ManagerStockpile {

private:

  const Config &cfg;

  double resource;

  emp::vector<double> contrib_resource;
  emp::vector<size_t> harvest_withdrawals;

  size_t decline_sharing;

public:

  ManagerStockpile(
    const Config &cfg_
  ) : cfg(cfg_)
  , contrib_resource(Cardi::Dir::NumDirs)
  , harvest_withdrawals(cfg_.NLEV())
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
    harvest_withdrawals[lev] += (amt < 0.0);
    resource += amt;
  }

  void InternalAcceptResource(const double amt) {
    emp_assert(amt > 0);
    resource += amt;
  }

  std::function<void()> MakeRepRefunder() {
    return [&](){ resource += cfg.REP_THRESH(); };
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
    for(size_t dir = 0; dir < contrib_resource.size(); ++dir) {
      resource += contrib_resource[dir];
      contrib_resource[dir] = 0.0;
    }
  }

  bool IsBankrupt() const {
    return cfg.KILL_THRESH() >= resource;
  }

  bool CheckAcceptSharing() const {
    return !decline_sharing;
  }

  void SetAcceptSharing(const size_t set) {
    decline_sharing = set;
  }

  void ResolveNextAcceptSharing(const size_t update) {
    if (update % cfg.ENV_TRIG_FREQ() == 0 && decline_sharing) --decline_sharing;
  }

  void Reset() {
    decline_sharing = 0;
    resource = 0.0;
    std::fill(std::begin(contrib_resource), std::end(contrib_resource), 0.0);
    std::fill(
      std::begin(harvest_withdrawals),
      std::end(harvest_withdrawals),
      0
    );
  }

  void ApplyBaseInflow() {
    resource += cfg.BASE_RESOURCE_INFLOW();
  }

  size_t QueryHarvestWithdrawals(size_t lev) const {
    return harvest_withdrawals[lev];
  }

  void ResetHarvestWithdrawals(size_t lev) {
    harvest_withdrawals[lev] = 0;
  }

};
