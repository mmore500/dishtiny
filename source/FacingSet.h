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
  size_t facing_channelsensor;

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

    facing_channelsensor = facings.size();
    for(size_t j = 0; j < cfg.NLEV() + 1; ++j) {
      facings.push_back(Cardi::Spin(local_rng));
    }

  }

  /* accessor function */
  const size_t GetNumFacings() {
    return facings.size() + 1;
  }
  /* accessor function */
  const size_t GetReproduction() {
    return facing_reproduction;
  }
  /* accessor function */
  const size_t GetSendResource() {
    return facing_sendresource;
  }
  /* accessor function */
  const size_t GetSendMessage() {
    return facing_sendmessage;
  }
  /* accessor function */
  const size_t GetChannelSensor() {
    return facing_channelsensor;
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
      emp_assert(which < facings.size());
      emp_assert(false, "Not implemented!"); // TODO
    }
  }

  void Cw(size_t which) {
    if (which == GetAllFacings()) {
      for(size_t i = 0; i < facings.size(); ++i) {
        Cw(i);
      }
    } else {
      emp_assert(which < facings.size());
      emp_assert(false, "Not implemented!"); // TODO
    }
  }

  void Unify(size_t which) {
    /* point all facings towards the desired facing */
    //TODO
    for(size_t i = 0; i < facings.size(); ++i) {
      emp_assert(false, "Not implemented!"); // TODO
      // Cardi::Dir rand = gen rand;
      // self[i] = rand;
    }
  }

  void Spin(size_t which) {
    if (which == GetAllFacings()) {
      for(size_t i = 0; i < facings.size(); ++i) {
        Spin(i);
      }
    } else {
      emp_assert(which < facings.size());
      facings[which] = Cardi::Spin(local_rng);
    }
  }
};
