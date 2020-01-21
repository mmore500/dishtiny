#pragma once

#include <mutex>

#include "ManagerConnection.h"

ManagerConnection::ManagerConnection(
  const size_t location_,
  const std::function<FrameCell &(const size_t)> cell_getter_,
  const emp::vector<emp::Ptr<ManagerChannel>> & mcs_,
  const Config &cfg_,
  emp::Random &local_rng_
) : location(location_)
, geom(cfg_)
, cell_getter(cell_getter_)
, mcs(mcs_)
, cfg(cfg_)
, local_rng(local_rng_)
, connection_prune_count(0)
{
  Reset();
  shuffler = emp::vector<size_t>(
    [](){
      emp::vector<size_t> res(Cardi::Dir::NumDirs);
      std::iota(std::begin(res), std::end(res), 0);
      return res;
    }()
  );
  emp_assert(location < geom.GetLocalSize());
}

void ManagerConnection::Reset() {
  query.clear();
  fledgling.clear();
  while (developed.size()) {
    RemoveOutgoingConnection(std::begin(developed)->first);
  }
  aging_param = default_aging_param;
  exploit_param = default_exploit_param;
  development_param = default_development_param;
  connection_prune_count = 0;
}

std::unordered_multimap<
  size_t,
  std::reference_wrapper<FrameCell>
> & ManagerConnection::ViewDeveloped() {
  return developed;
}


void ManagerConnection::SetAgingParam(const double request) {
  aging_param = default_aging_param + request;
  }

void ManagerConnection::SetExploitParam(const double request) {
  exploit_param = default_exploit_param + request;
}

void ManagerConnection::SetDevelopmentParam(const double request) {
  development_param = default_development_param + request;
}

void ManagerConnection::SetSensingParam(const double request) {
  development_param = default_sensing_param + 4 * std::tanh(request / 4);
}

void ManagerConnection::AddQuery(
  const Config::matchbin_t::query_t & tag,
  const size_t coundown_timer,
  const double match_impact
) {
  query[tag] = std::tuple{coundown_timer, match_impact};
}

void ManagerConnection::TryAddFledgling() {
  if (fledgling.size() + developed.size() < cfg.MAX_CONNECTIONS()) {
    fledgling.emplace_back();
    for (size_t probe = 0; probe < cfg.FLEDGLING_COPIES(); ++probe) {
      fledgling.back().emplace_back(location, 0.0);
    }
  }
}

void ManagerConnection::SearchAndDevelop() {

  // decrement query timers and remove them if they're old
  // adapted from https://stackoverflow.com/a/9210110
  for (auto it = std::begin(query); it != std::end(query); ) {
    auto&& [tag, tup] = *it;
    if (std::get<0>(tup)) {
      --std::get<0>(tup);
      ++it;
    } else {
      it = query.erase(it);
    }
  }

  for (size_t f = 0; f < fledgling.size(); ++f) {

    auto & probes = fledgling[f];

    // random walk all probes
    for (auto & probe : probes) {

      emp::Shuffle(local_rng, shuffler);
      for (const auto & dir : shuffler) {

        const size_t dest = geom.CalcLocalNeigh(std::get<0>(probe), dir);
        if (mcs[location]->CheckMatch(*mcs[dest], cfg.NLEV()-1)) {
          std::get<0>(probe) = dest;
          emp_assert(dest < geom.GetLocalSize());
          break;
        }

      }

    }

    // find most successful probe
    const auto & best_probe = *std::max_element(
      std::begin(probes),
      std::end(probes),
      [this](const auto & first, const auto &second) {
        // second is BETTER if it has a higher score
        // OR the first has drifted out of the channel group but the second
        // hasn't
        return (
          std::get<1>(first) < std::get<1>(second)
          || (
            ! mcs[location]->CheckMatch(*mcs[std::get<0>(first)], cfg.NLEV()-1)
            && mcs[location]->CheckMatch(
              *mcs[std::get<0>(second)], cfg.NLEV()-1
            )
          )
        );
      }
    );

    // if best probe's activation is past the threshold
    // AND it's somewhere other than the source cell
    // AND it's still in the same channel group as the source cell
    // then it develops
    if (
      const size_t final_loc = std::get<0>(best_probe);
      std::get<1>(best_probe) >= development_param
      && location != final_loc
      && mcs[location]->CheckMatch(*mcs[final_loc], cfg.NLEV()-1)
    ) {

      emp_assert(final_loc < geom.GetLocalSize());


      {
        auto & in_mutex = cell_getter(final_loc).incoming_connection_mutex;
        auto & out_mutex = outgoing_connection_mutex;

        // acquire both mutexes in one swoop to prevent deadlock
        std::lock(in_mutex, out_mutex);
        std::lock_guard<std::mutex> in_guard(in_mutex, std::adopt_lock);
        std::lock_guard<std::mutex> out_guard(out_mutex, std::adopt_lock);

        cell_getter(final_loc).RegisterIncomingConnection(location);

        developed.emplace(
          final_loc,
          cell_getter(final_loc)
        );
      }

      // swap 'n pop out fledgling entry
      std::swap(
        fledgling[f],
        fledgling.back()
      );
      fledgling.pop_back();
      --f;

      continue;
    }

    for (auto & probe : probes) {
      // set worst probes and out of channel group probes
      // to the most successful probe or, failing that, to the spawning cell
      if (
        std::get<1>(best_probe) - exploit_param > std::get<1>(probe)
        || ! mcs[location]->CheckMatch(*mcs[std::get<0>(probe)], cfg.NLEV()-1)
      ) {
        if (
          mcs[location]->CheckMatch(*mcs[std::get<0>(best_probe)], cfg.NLEV()-1)
        ) probe = best_probe;
        else probe = {location, 0.0};
      }

      // each probe uses tags to sense the favorability of its current location
      const size_t loc = std::get<0>(probe);
      auto & dest = cell_getter(loc).GetSpiker().GetMembrane();
      for (const auto & [tag, tup] : query) {
        const double match_impact = std::get<1>(tup);
        const auto res = dest.GetVals(dest.Match(tag));
        const double activation = static_cast<double>(std::count_if(
          std::begin(res),
          std::end(res),
          [](const auto & v){ return v; }
        )) / static_cast<double>(res.size());
        std::get<1>(probe) += activation * sensing_param * match_impact;
      }
      std::get<1>(probe) += aging_param;

    }

  }

}

void ManagerConnection::DeleteOutgoingConnection(size_t loc) {
  emp_assert(developed.size());
  emp_assert(developed.count(loc));
  developed.erase(developed.find(loc));
}

void ManagerConnection::RemoveOutgoingConnection(size_t loc) {

  emp_assert(loc < geom.GetLocalSize());

  auto & in_mutex = cell_getter(loc).incoming_connection_mutex;
  auto & out_mutex = outgoing_connection_mutex;

  // acquire both mutexes in one swoop to prevent deadlock
  std::lock(in_mutex, out_mutex);
  std::lock_guard<std::mutex> in_guard(in_mutex, std::adopt_lock);
  std::lock_guard<std::mutex> out_guard(out_mutex, std::adopt_lock);

  emp_assert(developed.size());
  emp_assert(developed.count(loc));
  cell_getter(loc).DeleteIncomingConnection(location);
  DeleteOutgoingConnection(loc);
}

void ManagerConnection::PruneOutgoingConnection() {
  connection_prune_count = 1;
}

void ManagerConnection::DoPrune() {
  while (developed.size() && connection_prune_count) {
    RemoveOutgoingConnection(std::begin(developed)->first);
    --connection_prune_count;
  }
}
