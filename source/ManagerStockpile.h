#pragma once

#include <numeric>

#include "base/assert.h"

#include "Config.h"

class ManagerStockpile {

private:

  const Config &cfg;

  double resource;

  emp::vector<double> contrib_resource;

public:

  ManagerStockpile(
    const Config &cfg_
  ) : cfg(cfg_)
  , contrib_resource(Cardi::Dir::NumDirs)
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

  double RequestResourceFrac(const double frac) {
    emp_assert(frac >= 0 && frac <= 1);
    if (resource > 0 ) {
      double amt = resource * frac;
      resource -= amt;
      return amt;
    } else {
      return 0;
    }
  }

  void InternalApplyHarvest(const double amt) {
    resource += amt;
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

  void Reset() {
    resource = 0.0;
    for(size_t dir = 0; dir < contrib_resource.size(); ++dir) {
      contrib_resource[dir] = 0.0;
    }
  }

};
