#pragma once

#include "base/vector.h"
#include "tools/Random.h"

#include "CustomConfig.h"
#include "DishtinyConfig.h"

class Organism {
private:
  emp::Ptr<emp::Random> rand;

  emp::vector<double> endowments;
  emp::vector<double> res_pools;
  emp::vector<double> off_overs;
  emp::vector<double> off_ch_caps;

  emp::Ptr<CustomConfig> cconfig;

public:
  Organism(
    emp::Ptr<emp::Random> _r,
    DishtinyConfig& dconfig,
    emp::Ptr<CustomConfig> _cconfig)
  : rand(_r)
  , endowments(dconfig.NLEV()+1)
  , res_pools(dconfig.NLEV()+1)
  , off_overs(dconfig.NLEV())
  , off_ch_caps(dconfig.NLEV())
  , cconfig(_cconfig)
  {
    // initialize genetic information
    for (size_t lev = 0; lev < dconfig.NLEV(); ++lev) {
      off_ch_caps[lev] = init_off_ch_cap(lev);
      endowments[lev] = init_endowment(lev);
      res_pools[lev] = init_res_pool(lev);
      off_overs[lev] = init_off_over(lev);
    }

    endowments[dconfig.NLEV()] = init_endowment(dconfig.NLEV());
    res_pools[dconfig.NLEV()] = init_res_pool(dconfig.NLEV());

    bal_res_pool();

  }

  Organism(const Organism& par)
  : rand(par.rand)
  , endowments(par.endowments.size())
  , res_pools(par.res_pools.size())
  , off_overs(par.off_overs.size())
  , off_ch_caps(par.off_ch_caps.size())
  , cconfig(par.cconfig)
  {
    // initialize genetic information
    for (size_t lev = 0; lev < endowments.size(); ++lev) {
      endowments[lev] = par.endowments[lev];
    }

    for (size_t lev = 0; lev < res_pools.size(); ++lev) {
      res_pools[lev] = par.res_pools[lev];
    }

    for (size_t lev = 0; lev < off_overs.size(); ++lev) {
      off_overs[lev] = par.off_overs[lev];
    }

    for (size_t lev = 0; lev < off_ch_caps.size(); ++lev) {
      off_ch_caps[lev] = par.off_ch_caps[lev];
    }

  }

  inline bool operator==(const Organism& other) const {
    return (this == &other);
  }

  bool operator!=(const Organism& other) const {
    return !(operator==(other));
  }

  inline void DoMutations(emp::Random& x) {
    // mutate genetic information
    for (size_t lev = 0; lev < endowments.size(); ++lev) {
      endowments[lev] = mut_endowment(endowments[lev], lev);
    }

    for (size_t lev = 0; lev < res_pools.size(); ++lev) {
      res_pools[lev] = mut_res_pool(res_pools[lev], lev);
    }

    for (size_t lev = 0; lev < off_overs.size(); ++lev) {
      off_overs[lev] = mut_off_over(off_overs[lev], lev);
    }

    for (size_t lev = 0; lev < off_ch_caps.size(); ++lev) {
      off_ch_caps[lev] = mut_off_ch_cap(off_ch_caps[lev], lev);
    }

    bal_res_pool();

  }

  ~Organism() { ; }

  inline double GetEndowment(size_t lev) const {
    return endowments[lev];
  }

  inline size_t GetEndowmentDepth() const {
    return endowments.size();
  }

  inline double GetResPool(size_t lev) const {
    return res_pools[lev];
  }

  inline size_t GetResPoolDepth() const {
    return res_pools.size();
  }

  inline double GetOffOver(size_t lev) const {
    return off_overs[lev];
  }

  inline size_t GetOffOverDepth() const {
    return off_overs.size();
  }

  inline double GetOffChCap(size_t lev) const {
    return off_ch_caps[lev];
  }

  inline size_t GetOffChCapDepth() const {
    return off_ch_caps.size();
  }

private:
  inline void bal_res_pool() {
    // we can assume res_pools values are non-negative
    double s = 0.0;
    for (size_t i = 0; i < res_pools.size(); ++i) {
      s += res_pools[i];
    }
    if (s > 0) {
      for (size_t i = 0; i < res_pools.size(); ++i) {
        res_pools[i] /= s;
      }
    } else {
      for (size_t i = 0; i < res_pools.size(); ++i) {
        res_pools[i] = 1.0 / res_pools.size();
      }
    }
  }

  inline double init_off_ch_cap(size_t lev) {
    return rand->GetDouble(48.0);
  }

  inline double init_endowment(size_t lev) {
    return rand->GetDouble(3.0);
  }

  inline double init_res_pool(size_t lev) {
    return rand->GetDouble(1.0);
  }

  inline double init_off_over(size_t lev) {
    return std::max(std::min(rand->GetDouble(-0.5, 1.5), 1.0), 0.0);
  }

  inline double mut_off_ch_cap(double cur, size_t lev) {
    double rval = cur;
    if (rand->GetDouble() < cconfig->PM_OFF_CH_CAP[lev]) {
      rval = std::max(cur + rand->GetRandNormal(0.0, 24.0), 0.0);
    }
    return rval;
  }

  inline double mut_endowment(double cur, size_t lev) {
    double rval = cur;
    if (rand->GetDouble() < cconfig->PM_OFF_CH_CAP[lev]) {
      rval = std::max(cur + rand->GetRandNormal(0.0, 10.0), 0.0);
    }
    return rval;
  }

  inline double mut_res_pool(double cur, size_t lev) {
    double rval = cur;
    if (rand->GetDouble() < cconfig->PM_OFF_CH_CAP[lev]) {
      rval = std::max(cur + rand->GetRandNormal(0.0, 0.2), 0.0);
    }
    return rval;
  }

  inline double mut_off_over(double cur, size_t lev) {
    double rval = cur;
    if (rand->GetDouble() < cconfig->PM_OFF_CH_CAP[lev]) {
      rval = init_off_over(lev);
    }
    return rval;
  }

};
