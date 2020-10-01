#pragma once

#include <mutex>

#include "ManagerConnection.hpp"

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
  queries.clear();
  fledglings.clear();
  while (developed.size()) {
    RemoveOutgoingConnection(std::begin(developed)->first);
  }
  aging_param = default_aging_param;
  exploit_param = default_exploit_param;
  development_param = default_development_param;
  connection_prune_count = 0;
}

const std::unordered_multimap<
  size_t,
  std::reference_wrapper<FrameCell>
> & ManagerConnection::ViewDeveloped() const {
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
  queries[tag] = { .timer = coundown_timer, .impact = match_impact };
}

// only call this at ENV_TRIG_FREQ
void ManagerConnection::DecayQueries() {
  // decrement query timers and remove them if they're old
  // adapted from https://stackoverflow.com/a/9210110
  for (
    auto it = std::begin(queries);
    it != std::end(queries);
    // advance handled within loop
  ) {
    auto&& [tag, query] = *it;
    if (query.timer) {
      --query.timer;
      ++it;
    } else {
      it = queries.erase(it);
    }
  }
}

void ManagerConnection::TryAddFledgling(const size_t connection_cap) {
  if (
    fledglings.size() + developed.size()
    < std::min(cfg.MAX_CONNECTIONS(), connection_cap)
  ) {
    fledglings.emplace_back();
    for (size_t probe = 0; probe < cfg.FLEDGLING_COPIES(); ++probe) {
      fledglings.back().push_back(Probe{.location=location, .activation=0.0});
    }
  }
}


void ManagerConnection::SearchAndDevelop() {

  RandomStepFledgling();

  for (size_t f = 0; f < fledglings.size(); ++f) {

    auto & probes = fledglings[f];

    // find most successful probe
    const auto & best_probe = *std::max_element(
      std::begin(probes),
      std::end(probes),
      [this](const auto & first, const auto &second) {
        // second is BETTER if it has a higher score
        // OR the first has drifted out of the channel group but the second
        // hasn't
        return (
          first.activation < second.activation
          || (
            ! mcs[location]->CheckMatch(*mcs[first.location], cfg.NLEV()-1)
            && mcs[location]->CheckMatch(
              *mcs[second.location], cfg.NLEV()-1
            )
          )
        );
      }
    );

    if (TryDevelopProbe(best_probe)) {
      // swap 'n pop out fledglings entry
      std::swap(
        fledglings[f],
        fledglings.back()
      );
      fledglings.pop_back();
      --f;

      continue;
    }

    for (auto & probe : probes) {

      // each probe uses tags to sense the favorability of its current location
      const size_t loc = probe.location;
      auto & membrane = cell_getter(loc).GetSpiker().GetExternalMembrane();
      for (const auto & [tag, query] : queries) {
        const double match_impact = query.impact;
        const double match_result = membrane.LookupProportion(tag);
        probe.activation += match_result * sensing_param * match_impact;
      }
      probe.activation += aging_param;

      // ensure that cumulative activation doesn't go negative
      probe.activation = std::max(probe.activation, 0.0);

    }

    for (auto & probe : probes) {
      // set worst probes and out of channel group probes
      // to the most successful probe or, failing that, to the spawning cell
      if (
        best_probe.activation - exploit_param > probe.activation
        || ! mcs[location]->CheckMatch(*mcs[probe.location], cfg.NLEV()-1)
      ) {
        if (probe == best_probe) probe = {location, 0.0};
        else probe = best_probe;

      }

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

void ManagerConnection::PruneOutgoingConnection(const size_t connection_floor) {
  if (developed.size() > connection_floor) {
    connection_prune_count = 1;
  }
}

void ManagerConnection::DoPrune() {
  while (developed.size() && connection_prune_count) {
    RemoveOutgoingConnection(std::begin(developed)->first);
    --connection_prune_count;
  }
}

const ManagerConnection::fledgling_t & ManagerConnection::ViewFledgling() const {
  return fledglings;
}

size_t ManagerConnection::GetOutgoingConnectionCount() const {
  return developed.size();
}

void ManagerConnection::RandomStepFledgling() {
  for (size_t f = 0; f < fledglings.size(); ++f) {

    auto & probes = fledglings[f];

    // random step all probes
    for (auto & probe : probes) {

      emp::Shuffle(local_rng, shuffler);
      for (const auto & dir : shuffler) {

        const size_t dest = geom.CalcLocalNeigh(probe.location, dir);
        if (mcs[location]->CheckMatch(*mcs[dest], cfg.NLEV()-1)) {
          probe.location = dest;
          emp_assert(dest < geom.GetLocalSize());
          break;
        }

      }

    }
  }
}

void ManagerConnection::DevelopConnection(const size_t destination) {
  auto & in_mutex = cell_getter(destination).incoming_connection_mutex;
  auto & out_mutex = outgoing_connection_mutex;

  // acquire both mutexes in one swoop to prevent deadlock
  std::lock(in_mutex, out_mutex);
  std::lock_guard<std::mutex> in_guard(in_mutex, std::adopt_lock);
  std::lock_guard<std::mutex> out_guard(out_mutex, std::adopt_lock);

  cell_getter(destination).RegisterIncomingConnection(location);

  developed.emplace(
    destination,
    cell_getter(destination)
  );

}

bool ManagerConnection::TryDevelopProbe(const Probe & best_probe) {
  // if best probe's activation is past the threshold
  // AND it's somewhere other than the source cell
  // AND it's still in the same channel group as the source cell
  // then it develops
  if (
    const size_t final_loc = best_probe.location;
    best_probe.activation >= development_param
    && location != final_loc
    && mcs[location]->CheckMatch(*mcs[final_loc], cfg.NLEV()-1)
  ) {

    emp_assert(final_loc < geom.GetLocalSize());

    DevelopConnection(final_loc);

    return true;

  }

  return false;

}

double ManagerConnection::CalcMeanConnectionLength() const {

  const double x1 = geom.GetLocalX(location);
  const double y1 = geom.GetLocalY(location);

  double res = 0.0;

  for (const auto & [idx, cell] : developed) {

    const double x2 = geom.GetLocalX(idx);
    const double y2 = geom.GetLocalY(idx);

    const double dx = std::min(
      std::abs(x1 - x2),
      std::min(x1, x2) + cfg.GRID_W() - std::max(x1, x2)
    );
    const double dy = std::min(
      std::abs(y1 - y2),
      std::min(y1, y2) + cfg.GRID_H() - std::max(y1, y2)
    );

    res += dx * dx + dy * dy;

  }

  return developed.size() ? res / developed.size() : res;

}
