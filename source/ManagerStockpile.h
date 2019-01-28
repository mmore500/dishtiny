#pragma once

#include "Config.h"

class ManagerStockpile {

  double resource;

  double next_resource;

  double contrib_resource;

  ManagerStockpile(
    Config &cfg
  ) : resource(cfg.START_RESOURCE())
  , contrib_resource(0)
  { ; }

  double RequestResourceAmt(double amt){
    emp::assert(amt >= 0);
    if (resource > 0) {
      double actual = std::min(resource, amt);
      resource -= actual;
      return actual;
    } else {
      return 0;
    }
    return resource;
  }

  double RequestResourceFrac(double frac){
    emp::assert(frac >= 0 && frac <= 1);
    if (resource > 0 ) {
      double amt = resource * frac;
      resource -= amt;
      return amt;
    } else {
      return 0;
    }
  }

  void ResolveRequests() {
    cur_resource += contrib_resource;
    contrib_resource = 0;
  }

  // TODO serialize and deserialiae


};
