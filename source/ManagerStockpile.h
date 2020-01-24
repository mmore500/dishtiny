#pragma once

#include <numeric>
#include <limits>
#include <mutex>

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

  std::mutex spike_contribute_mutex;

  mutable std::mutex resource_mutex;

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
    std::lock_guard<std::mutex> lock(resource_mutex);
    emp_assert(std::isfinite(resource));
    return resource;
  }

  double RequestResourceAmt(const double amt, const double reserve = 0.0) {

    std::lock_guard<std::mutex> lock(resource_mutex);

    emp_assert(amt >= 0.0); emp_assert(std::isfinite(amt));
    emp_assert(reserve >= 0.0); emp_assert(std::isfinite(reserve));
    emp_assert(std::isfinite(resource));

    const double available = std::max(0.0, resource - reserve);
    emp_assert(available >= 0.0); emp_assert(std::isfinite(available));

    const double dispensed = std::min(available, amt);
    emp_assert(dispensed >= 0.0); emp_assert(std::isfinite(dispensed));

    resource -= dispensed;

    //if you dispensed something, resource >= 0
    emp_assert(!dispensed || resource >= 0.0, dispensed, resource);
    emp_assert(std::isfinite(resource));

    return dispensed;

  }

  double RequestResourceFrac(const double frac, const double reserve = 0.0) {
    emp_assert(frac >= 0.0 && frac <= 1.0); emp_assert(std::isfinite(frac));
    emp_assert(reserve >= 0.0); emp_assert(std::isfinite(reserve));

    return RequestResourceAmt(frac * QueryResource(), reserve);
  }

  void InternalApplyHarvest(const size_t lev, const double amt) {
    std::lock_guard<std::mutex> lock(resource_mutex);
    // if ANY level expired, can't collect resource
    const size_t exp = expchecker(lev);
    harvest_withdrawals[lev] += (amt <= 0.0);
    if (exp <= cfg.EXP_GRACE_PERIOD()) {
      // grace period to allow a few reproductions without sharing
      resource += amt;
    } else {
      // no reward, maybe penalty
      resource += std::min(0.0, amt);
    }
    emp_assert(std::isfinite(resource));

  }

  void InternalAcceptResource(const double amt) {
    std::lock_guard<std::mutex> lock(resource_mutex);
    emp_assert(amt >= 0); emp_assert(std::isfinite(amt));
    resource += amt;
    emp_assert(std::isfinite(resource));
  }

  std::function<void()> MakeRepRefunder() {
    return [&](){ refund_resource += cfg.REP_THRESH(); };
  }

  void ExternalContribute(const double amt, const size_t incoming_dir) {
    emp_assert(amt >= 0); emp_assert(std::isfinite(amt));
    if (incoming_dir == Cardi::Dir::NumDirs) {
      std::lock_guard<std::mutex> lock(spike_contribute_mutex);
      contrib_resource[incoming_dir] += amt;
    } else {
      contrib_resource[incoming_dir] += amt;
    }
  }

  double QueryExternalContribute(const size_t incoming_dir) const {
    return contrib_resource[incoming_dir];
  }

  double QueryTotalContribute() const {
    return std::accumulate(
      std::begin(contrib_resource),
      std::end(contrib_resource),
      0.0
    );
  }

  void ResolveExternalContributions() {
    std::lock_guard<std::mutex> lock(resource_mutex);

    emp_assert(std::all_of(
      std::begin(contrib_resource),
      std::end(contrib_resource),
      [](const auto val){ return val >= 0.0; }
    ));
    resource += refund_resource;
    refund_resource = 0.0;
    resource += std::accumulate(
      std::begin(contrib_resource),
      std::end(contrib_resource),
      0.0
    );
    std::fill(
      std::begin(contrib_resource),
      std::end(contrib_resource),
      0.0
    );
    emp_assert(std::isfinite(resource));
  }

  bool IsBankrupt() const {
    return cfg.KILL_THRESH() >= QueryResource();
  }

  void Reset() {
    std::lock(resource_mutex, spike_contribute_mutex);
    std::lock_guard<std::mutex>(resource_mutex, std::adopt_lock);
    std::lock_guard<std::mutex>(spike_contribute_mutex, std::adopt_lock);

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
    std::lock_guard<std::mutex> lock(resource_mutex);
    resource += cfg.BASE_RESOURCE_INFLOW();
    emp_assert(std::isfinite(resource));
  }

  void ApplyDecay(){
    std::lock_guard<std::mutex> lock(resource_mutex);
    if (resource > 0) resource *= cfg.RESOURCE_DECAY();
    emp_assert(std::isfinite(resource));
  }

  size_t QueryHarvestWithdrawals(size_t lev) const {
    return harvest_withdrawals[lev];
  }

  void ResetHarvestWithdrawals(size_t lev) {
    harvest_withdrawals[lev] = 0;
  }

};
