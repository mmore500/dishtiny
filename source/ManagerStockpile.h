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
  emp::vector<size_t> harvest_withdrawals;

  emp::vector<size_t> decline_sharing;
  emp::vector<std::function<void()>> sharing_doers;

  std::function<size_t(size_t)> expchecker;

public:

  ManagerStockpile(
    std::function<size_t(size_t)> expchecker_,
    const Config &cfg_
  ) : cfg(cfg_)
  , contrib_resource(Cardi::Dir::NumDirs)
  , harvest_withdrawals(cfg_.NLEV())
  , decline_sharing(Cardi::Dir::NumDirs)
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
    const size_t exp = expchecker(lev);
    harvest_withdrawals[lev] += (amt < 0.0) || (exp > 3);
    if (exp < 4) {
      // grace period to allow a few reproduction without sharing
      resource += amt;
    } else if (exp <= 6) {
      // no reward, maybe penalty
      resource += std::min(0.0, amt);
    } else {
      // resource += -std::abs(amt);
      // time to die
      resource -= std::numeric_limits<double>::infinity();
    }

  }

  void InternalAcceptResource(const double amt) {
    emp_assert(amt >= 0);
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

  bool CheckAcceptSharing(const size_t outgoing_dir) const {
    return !decline_sharing[outgoing_dir];
  }

  void SetAcceptSharing(const size_t outgoing_dir, const size_t set) {
    decline_sharing[outgoing_dir] = set;
  }

  void ResolveNextAcceptSharing(const size_t update) {
    if (update % cfg.ENV_TRIG_FREQ()) return;
    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      if (decline_sharing[dir]) --decline_sharing[dir];
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
    std::fill(std::begin(decline_sharing), std::end(decline_sharing), 0);
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
