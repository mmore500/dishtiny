#pragma once

#include "base/vector.h"

#include "Cardi.h"
#include "DishtinyConfig.h"

class FacingSet {

  private:
    emp::vector<Cardi::Dir> facings;

    size_t facing_reproduction;
    size_t facing_sendresource;
    size_t facing_sendmessage;
    size_t facing_channelsensor;

  public:
    FacingSet(DishtinyConfig &config) {

      facing_reproduction = facings.size();
      for(size_t j = 0; j < config.NLEV() + 1; ++j) {
        facings.push_back(Cardi::Dir);
      }

      facing_sendresource = facings.size();
      facings.push_back(Cardi::Dir);

      facing_sendmessage = facings.size();
      facings.push_back(Cardi::Dir);

      facing_channelsensor = facings.size();
      for(size_t j = 0; j < config.NLEV() + 1; ++j) {
        facings.push_back(Cardi::Dir);
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
      return facing_ChannelSensor;
    }
    /* accessor function */
    const size_t GetAllFacings() {
      return facings.size();

    }

    const Cardi::Dir Get(size_t which) {
      if (which == GetAllFacings()) {

      } else {
        emp::assert(which < GetAllFacings());

        emp::assert(false, "Not implemented!"); // TODO
      }
    }

    void Ccw(size_t which) {
      if (which == GetAllFacings()) {
        for(size_t i = 0; i < self.size(); ++i) {
          self->Ccw(i);
        }
      } else {
        emp::assert(which < self.size());
        emp::assert(false, "Not implemented!"); // TODO
      }
    }

    void Cw(size_t which) {
      if (which == GetAllFacings()) {
        for(size_t i = 0; i < self.size(); ++i) {
          self->Cw(i);
        }
      } else {
        emp::assert(which < self.size());
        emp::assert(false, "Not implemented!"); // TODO
      }
    }

    void Unify(emp::Random &rng, i) {
      for(size_t i = 0; i < self.size(); ++i) {
        Cardi::Dir rand = gen rand;
        self[i] = rand;
      }
    }

    void Spin(emp::Random &rng, size_t which) {
      if (which == GetAllFacings()) {
        for(size_t i = 0; i < self.size(); ++i) {
          // TODO
        }
      } else {
        emp::assert(which < self.size());
        emp::assert(false, "Not implemented!"); // TODO
      }
    }
}
