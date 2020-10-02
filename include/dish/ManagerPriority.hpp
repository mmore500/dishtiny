#pragma once
#ifndef DISH_MANAGERPRIORITY_HPP_INCLUDE
#define DISH_MANAGERPRIORITY_HPP_INCLUDE

#include <algorithm>
#include <optional>
#include <tuple>
#include <memory>

#include "../../third-party/Empirical/source/base/Ptr.h"
#include "../../third-party/Empirical/source/base/vector.h"
#include "../../third-party/Empirical/source/tools/Random.h"
#include "../../third-party/Empirical/source/tools/random_utils.h"

#include "ChannelPack.hpp"
#include "Config.hpp"
#include "Genome.hpp"
#include "FrameHardware.hpp"

namespace dish {

struct SirePack {

  size_t par_pos;
  size_t outgoing_dir;
  size_t incoming_dir;
  size_t replev;
  emp::vector<size_t> channel_gens;
  emp::vector<double> expirations;
  emp::vector<size_t> cell_gen;
  ChannelPack chanpack;
  Config::chanid_t prev_chan;
  // non-owning at first, then we make a copy to hand off if selected
  // to reproduce
  emp::Ptr<Genome> genome;

  std::optional<
    emp::vector<Config::matchbin_t::state_t>
  > matchbins;

  double par_wealth;

  std::optional<Config::tag_t> imprint;

};

class ManagerPriority {

private:
  const Config &cfg;

  emp::Random &local_rng;

  // incoming_dir -> SirePack
  emp::vector<std::optional<SirePack>> requests;

  // incoming_dir -> rep_lev -> pause_counter
  emp::vector<emp::vector<size_t>> pauses;

  std::optional<SirePack> pending_birth;

  emp::vector<std::function<void()>> refunders;

  // incoming_dir -> rep_lev
  // for data collection only
  emp::vector<int> rep_state;
  emp::vector<int> rep_state_dup;

public:
  ManagerPriority(
    const Config &cfg_,
    emp::Random &local_rng_,
    emp::vector<std::function<void()>> refunders_
  ) : cfg(cfg_)
  , local_rng(local_rng_)
  , requests(Cardi::Dir::NumDirs, std::nullopt)
  , pauses(Cardi::Dir::NumDirs, emp::vector<size_t>(cfg_.NLEV()+1, 0))
  , refunders(refunders_)
  , rep_state(Cardi::Dir::NumDirs)
  , rep_state_dup(Cardi::Dir::NumDirs)
  { ; }

  void Reset() {

    for (auto & dir : pauses) {
      std::fill(std::begin(dir), std::end(dir), 0);
    }
    pending_birth.reset();

    std::fill(std::begin(requests), std::end(requests), std::nullopt);

  }

  void ResolveUpdate() {

    std::fill(std::begin(requests), std::end(requests), std::nullopt);

    for (auto & dir : pauses) {
      for (size_t & pause : dir) if (pause) --pause;
    }
  }

  bool IsReprPaused(const size_t incoming_dir, const size_t rep_lev) const {
    return pauses[incoming_dir][rep_lev];
  }

  void PauseRepr(
    const size_t incoming_dir,
    const size_t rep_lev,
    const size_t dur
  ) {

    // set the pause value
    pauses[incoming_dir][rep_lev] = dur;

    // if there's no request, then we don't have any work to do
    if (!requests[incoming_dir]) return;

    // make a copy of the sirepack and then resubmit it
    SirePack sp = *requests[incoming_dir];
    requests[incoming_dir].reset();

    if (!AddRequest(sp)) refunders[incoming_dir]();

  }

  void ClearPauses(const size_t incoming_dir) {
    for (auto & pause : pauses[incoming_dir]) {
      if (pause) pause = 1;
    }
  }

  size_t CountRequests() {
    return std::count_if(
      std::cbegin(requests),
      std::cend(requests),
      [](const std::optional<SirePack> & sp){ return static_cast<bool>(sp); }
    );
  }

  // return whether an additional request was queued
  // i.e., whether reproduction cost should be assessed
  bool AddRequest(const SirePack & sp) {

    for (size_t l = sp.replev; l < cfg.NLEV() + 1; ++l) {
      if (pauses[sp.incoming_dir][l] || sp.expirations[l]) {
        // if repr paused or channel has expired, try the next higher replev
        continue;
      } else {
        const bool was_empty = !requests[sp.incoming_dir];
        // if request already exists, update request but don't charge for it
        requests[sp.incoming_dir] = sp;
        // be sure to register the correct replev
        requests[sp.incoming_dir]->replev = l;
        return was_empty;
      }
    }

    // no suitable replev was found, no reproduction request was queued
    return false;
  }

  void ResolveSire(const double cur_stockpile) {

    // set up reproduction attempts to be logged
    for (size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
      rep_state[d] = requests[d] ? requests[d]->replev : -1;
      rep_state_dup[d] = requests[d] ? requests[d]->replev : -1;
    }

    // how many valid requests are there?
    size_t n_req = CountRequests();

    while (n_req) {
      // pick a request at random
      const size_t choice = local_rng.GetUInt(n_req);
      size_t count = 0;
      const auto which = std::find_if(
        std::cbegin(requests),
        std::cend(requests),
        [&count, choice](const std::optional<SirePack> & sp) {
          return static_cast<bool>(sp) && count++ == choice;
        }
      );
      emp_assert(which != std::cend(requests));

      // was parent rich enough?
      if (
        !cfg.RESOURCE_ARMOR()
        || cur_stockpile <= 0.0
        || (*which)->par_wealth - cfg.REP_LEAD() > cur_stockpile
      ) {
        // set up the pending birth!
        pending_birth = *which;
        // make a copy of the genome to hand off to the World
        // also, because the parent genome might be overwritten
        pending_birth->genome = emp::NewPtr<Genome>(*(pending_birth->genome));

        // refund requests that don't get selected
        for (size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
          if (requests[d] && pending_birth->incoming_dir != d) {
            refunders[d]();
          }
        }
        return;
      } else {
        // if parent not rich enough, withdraw that request and try again
        n_req--;
        refunders[(*which)->incoming_dir]();
        requests[(*which)->incoming_dir].reset();
      }

    }

  }

  const std::optional<SirePack>& QueryPendingGenome() const {
    return pending_birth;
  }

  void ClearPendingGenome() {
    pending_birth.reset();
  }

  int ViewRepState(const size_t incoming_dir) {
    int res = -1;
    // make sure each reproduction attempt is only viewed once
    std::swap(rep_state[incoming_dir], res);
    return res;
  }

  // there are two data collection processes that need to vew RepState...
  int ViewRepStateDup(const size_t incoming_dir) {
    int res = -1;
    // make sure each reproduction attempt is only viewed once
    std::swap(rep_state_dup[incoming_dir], res);
    return res;
  }

};

} // namespace dish

#endif // #ifndef DISH_MANAGERPRIORITY_HPP_INCLUDE
