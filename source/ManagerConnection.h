#pragma once

#include "FrameCell.h"
#include "GeometryHelper.h"
#include "ManagerChannel.h"

class FrameCell;

class ManagerConnection {

private:

  emp::vector<size_t> shuffler;

  std::unordered_map<
    Config::matchbin_t::query_t,
    std::tuple<
      size_t, // countdown timer
      double // match impact
    >
  > query;

  // TODO random walk params

  //fledgling
  emp::vector<
    emp::vector<
      std::tuple<
        size_t, // location
        double // activation
      >
    >
  > fledgling;

  //developed
  std::unordered_multimap<
    size_t, //location
    std::reference_wrapper<FrameCell>
  > developed;

  const size_t location;
  const GeometryHelper geom;

  const double default_exploit_param = 2.0;
  const double default_development_param = 12.0;
  const double default_aging_param = 1.0;
  const double default_sensing_param = 1.0;
  double exploit_param;
  double aging_param;
  double development_param;
  double sensing_param;

  const std::function<FrameCell &(const size_t)> cell_getter;
  const emp::vector<emp::Ptr<ManagerChannel>> & mcs;

  const Config &cfg;

  emp::Random &local_rng;

public:

  ManagerConnection(
    const size_t location_,
    const std::function<FrameCell &(const size_t)> cell_getter_,
    const emp::vector<emp::Ptr<ManagerChannel>> & mcs_,
    const Config &cfg_,
    emp::Random &local_rng_
  );

  void Reset();

  std::unordered_multimap<
    size_t,
    std::reference_wrapper<FrameCell>
  > & ViewDeveloped();

  void SetAgingParam(const double request);

  void SetExploitParam(const double request);

  void SetDevelopmentParam(const double request);

  void SetSensingParam(const double request);

  void AddQuery(
    const Config::matchbin_t::query_t & tag,
    const size_t coundown_timer,
    const double match_impact
  );

  void TryAddFledgling();

  void SearchAndDevelop();

  void DeleteOutgoingConnection(size_t loc);

  void RemoveOutgoingConnection(size_t loc);

  void RemoveOutgoingConnection();

};
