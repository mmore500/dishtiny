#pragma once

#include "FrameCell.h"
#include "GeometryHelper.h"
#include "ManagerChannel.h"

class FrameCell;

struct Probe {
  size_t location;
  double activation;
  bool operator==(Probe const& other) const {
    return location == other.location && activation == other.activation;
  }
};

struct DevoQuery {
  size_t timer;
  double impact;
};

class ManagerConnection {

public:

  using fledgling_t = emp::vector<
    emp::vector<
      Probe
    >
  >;

private:

  emp::vector<size_t> shuffler;

  std::unordered_map<
    Config::matchbin_t::query_t,
    DevoQuery
  > queries;

  // TODO random walk params

  //fledgling
  fledgling_t fledglings;

  //developed
  std::unordered_multimap<
    size_t, //location
    std::reference_wrapper<FrameCell>
  > developed;

  const size_t location;
  const GeometryHelper geom;

  const double default_exploit_param = 1.0;
  const double default_development_param = 6.0;
  const double default_aging_param = 0.5;
  const double default_sensing_param = 10.0;
  double exploit_param;
  double aging_param;
  double development_param;
  double sensing_param;

  const std::function<FrameCell &(const size_t)> cell_getter;
  const emp::vector<emp::Ptr<ManagerChannel>> & mcs;

  const Config &cfg;

  emp::Random &local_rng;

  size_t connection_prune_count;

  void RandomStepFledgling();

  void DevelopConnection(const size_t destination);

  bool TryDevelopProbe(const Probe & best_probe);


public:

  mutable std::mutex outgoing_connection_mutex;

  ManagerConnection(
    const size_t location_,
    const std::function<FrameCell &(const size_t)> cell_getter_,
    const emp::vector<emp::Ptr<ManagerChannel>> & mcs_,
    const Config &cfg_,
    emp::Random &local_rng_
  );

  void Reset();

  const std::unordered_multimap<
    size_t,
    std::reference_wrapper<FrameCell>
  > & ViewDeveloped() const;

  void SetAgingParam(const double request);

  void SetExploitParam(const double request);

  void SetDevelopmentParam(const double request);

  void SetSensingParam(const double request);

  void AddQuery(
    const Config::matchbin_t::query_t & tag,
    const size_t coundown_timer,
    const double match_impact
  );

  void DecayQueries();

  void TryAddFledgling(const size_t connection_cap);

  void SearchAndDevelop();

  void DeleteOutgoingConnection(size_t loc);

  void RemoveOutgoingConnection(size_t loc);

  void PruneOutgoingConnection(const size_t connection_floor=0);

  void DoPrune();

  const fledgling_t & ViewFledgling() const;

  size_t GetOutgoingConnectionCount() const;

};
