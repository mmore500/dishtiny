#pragma once

#include "base/assert.h"

#include "Config.h"

class ManagerStockpile {

private:

  double resource;

  double contrib_resource;

  const Config &cfg;

public:

  ManagerStockpile(
    const Config &cfg_
  ) : cfg(cfg_)
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

  void ExternalContribute(const double amt) {
    emp_assert(amt >= 0);
    contrib_resource += amt;
  }

  void ResolveExternalContributions() {
    resource += contrib_resource;
    contrib_resource = 0;
  }

  bool IsBankrupt() const {
    return cfg.KILL_THRESH() >= resource;
  }

  void Reset() {
    resource = 0.0;
    contrib_resource = 0.0;
  }

};
