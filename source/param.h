#ifndef PARAM_H
#define PARAM_H

#include <limits>
#include <numeric>
#include <iostream>

#include "tools/Random.h"

#define QUIESCENT_MAX 4

#define NLEV 2

const double EVENT_VALS_L1[] = {6.0};
const double EVENT_VALS_L2[] = {6.0};
const double *EVENT_VALS[] = {EVENT_VALS_L1, EVENT_VALS_L2};
const int EVENT_VALS_LENS[] = {1, 1};

const int EVENT_RADII_L1[] = {4};
const int EVENT_RADII_L2[] = {9};
const int *EVENT_RADII[] = {EVENT_RADII_L1, EVENT_RADII_L2};

const double EVENT_PROBS_L1[] = {0.0005};
const double EVENT_PROBS_L2[] = {0.0002};
const double *EVENT_PROBS[] = {EVENT_PROBS_L1, EVENT_PROBS_L2};

#define VULNR_COST
const double V_COST_L1[] = {-5.0};
const double V_COST_L2[] = {-5.0};
const double *V_COST[] = {V_COST_L1, V_COST_L2};

//#define QUIES_COST
//#define BASEL_COST

#define GRID_H 60
#define GRID_W 60
#define GRID_A (GRID_H * GRID_W)

#define REP_THRESH 8
#define KILL_THRESH -11

#define CH_MAX 4194304

inline int init_ch(emp::Random& r) {
  return r.GetInt(1,CH_MAX);
}

inline int change_ch(int cur, emp::Random& r) {
  return init_ch(r);
}

inline float init_resource(emp::Random& r) {
  return REP_THRESH - 1;
}

#define CULL_TARGET (2 / (GRID_A))

#define CULL_PENALTY (5 * (GRID_A) * KILL_THRESH)

#define CULL_FREQ 2500

// genotype
#define PM_OFF_CH_CAP {0.0002, 0.0002}

#define PM_RES_POOL {0.0002, 0.0002, 0.0002}

#define PM_OFF_OVER {0.0002, 0.0002}

#define PM_ENDOWMENT {0.0002, 0.0002}

inline double init_off_ch_cap(int lev, emp::Random& r) {
  return r.GetDouble(24.0);
}

inline double init_endowment(int lev, emp::Random& r) {
  return r.GetDouble(3.0);
}

inline double init_res_pool(int lev, emp::Random& r) {
  return r.GetDouble(1.0);
}

inline double init_share_fit(int lev, emp::Random& r) {
  return r.GetDouble(1.0);
}

inline double init_off_over(int lev, emp::Random& r) {
  return r.GetDouble(1.0);
}

inline double mut_off_ch_cap(double cur, int lev, emp::Random& r) {
  return cur + r.GetRandNormal(0.0, 2.0);
}

inline double mut_endowment(double cur, int lev, emp::Random& r) {
  return std::max(cur + r.GetRandNormal(0.0, 3.0), 0.0);
}

inline double mut_res_pool(double cur, int lev, emp::Random& r) {
  return std::max(cur + r.GetRandNormal(0.0, 0.2), 0.0);
}

inline double mut_off_over(double cur, int lev, emp::Random& r) {
  return std::min(std::max(cur + r.GetRandNormal(0.0, 0.2), 0.0), 1.0);
}

inline void balance_res_pool(emp::vector<double> vals) {
  double s = 0.0;
  for (emp::vector<double>::iterator it = vals.begin(); it != vals.end(); it ++) {
    s += *it;
  }
  if (s > 0) {
    for (emp::vector<double>::iterator it = vals.begin(); it != vals.end(); it ++) {
      *it /= s;
    }
  } else {
    for (emp::vector<double>::iterator it = vals.begin(); it != vals.end(); it ++) {
      *it = 1.0 / vals.size();
    }
  }
}

#endif
