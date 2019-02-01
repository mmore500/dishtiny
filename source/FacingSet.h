#pragma once

#include "base/vector.h"
#include "tools/Random.h"

#include "Cardi.h"
#include "Config.h"

class FacingSet {

private:
  emp::Random &local_rng;

  emp::vector<size_t> facings;

  size_t facing_reproduction;
  size_t facing_sendresource;
  size_t facing_sendmessage;
  size_t facing_neighborsensor;

public:
  FacingSet(
    emp::Random &local_rng_,
    Config &cfg
  ) : local_rng(local_rng_) {

    facing_reproduction = facings.size();
    for(size_t j = 0; j < cfg.NLEV() + 1; ++j) {
      facings.push_back(Cardi::Spin(local_rng));
    }

    facing_sendresource = facings.size();
    facings.push_back(Cardi::Spin(local_rng));

    facing_sendmessage = facings.size();
    facings.push_back(Cardi::Spin(local_rng));

    facing_neighborsensor = facings.size();
    facings.push_back(Cardi::Spin(local_rng));

  }

  /* accessor function */
  const size_t GetNumFacings() {
    return facings.size() + 1;
  }
  /* accessor function */
  const size_t GetReproduction(size_t i) {
    return Get(facing_reproduction+i);
  }
  /* accessor function */
  const size_t GetSendResource() {
    return Get(facing_sendresource);
  }
  /* accessor function */
  const size_t GetSendMessage() {
    return Get(facing_sendmessage);
  }
  /* accessor function */
  // TODO also used to check resource in other cell
  const size_t GetNeighborSensor() {
    return Get(facing_neighborsensor);
  }

  /* accessor function */
  const size_t GetAllFacings() {
    return facings.size();

  }

  const size_t Get(size_t which) {
    return facings[which];
  }

  void Ccw(size_t which) {
    if (which == GetAllFacings()) {
      for(size_t i = 0; i < facings.size(); ++i) {
        Ccw(i);
      }
    } else {
      facings[which] = Cardi::Ccw[facings[which]];
    }
  }

  void Cw(size_t which) {
    if (which == GetAllFacings()) {
      for(size_t i = 0; i < facings.size(); ++i) {
        Cw(i);
      }
    } else {
      facings[which] = Cardi::Cw[facings[which]];
    }
  }

  void Unify(size_t which) {
    /* point all facings towards the desired facing */
    if (which == GetAllFacings()) {
      Spin(0);
      Unify(0);
    } else {
      for(size_t i = 0; i < facings.size(); ++i) {
        facings[i] = facings[which];
      }
    }
  }

  void Spin(size_t which) {
    if (which == GetAllFacings()) {
      for(size_t i = 0; i < facings.size(); ++i) {
        Spin(i);
      }
    } else {
      facings[which] = Cardi::Spin(local_rng);
    }
  }
};
