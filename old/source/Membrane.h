#pragma once

#include <algorithm>
#include <unordered_map>

#include "tools/Random.h"
#include "tools/MatchBin.h"

class Membrane {

  Config::matchbin_t matchbin;

  std::unordered_map<
    Config::matchbin_t::tag_t,
    Config::matchbin_t::uid_t
  > tags;

  std::unordered_map<
    Config::matchbin_t::uid_t,
    size_t
  > timers;

public:

  Membrane(emp::Random & rand) : matchbin(rand) { ; }

  void Put(
    const Config::tag_t & affinity,
    const bool value,
    const size_t lifespan
  ) {

    // if a particular affinity is already in the MatchBin, take it out
    if (
      const auto &existing = tags.find( affinity );
      existing != std::end(tags)
    ) {
      matchbin.Delete(existing->second);
      timers.erase(existing->second);
      tags.erase(existing);
    }

    emp_assert(tags.find(affinity) == std::end(tags));

    // put new affinity into the MatchBin
    const auto uid = matchbin.Put(value, affinity);

    tags.insert({
      affinity,
      uid
    });

    timers.insert({
      uid,
      lifespan
    });

  }

  void Decay() {


    matchbin.DecayRegulators();

    // age entries and remove expired entries
    for (
      auto it = std::begin(timers);
      it != std::end(timers);
      // handled within for loop
    ) {
      auto & [uid, timer] = *it;
      if (timer) {
        --timer;
        ++it;
      } else {
        tags.erase(matchbin.GetTag(uid));
        matchbin.Delete(uid);
        it = timers.erase(it);
      }
    }

  }

  bool LookupLaxAllOf(const Config::tag_t & affinity) {
    const auto res = matchbin.GetVals(
      matchbin.Match(affinity)
    );
    return std::all_of(std::begin(res), std::end(res), [](auto a){return a;});
  }

  bool LookupStrictAllOf(const Config::tag_t & affinity) {
    const auto res = matchbin.GetVals(
      matchbin.Match(affinity)
    );
    return (
      res.size() &&
      std::all_of(std::begin(res), std::end(res), [](auto a){return a;})
    );
  }

  double LookupProportion(const Config::tag_t & affinity) {

    const auto res = matchbin.GetVals(
      matchbin.Match(affinity)
    );

    const size_t yups = std::count_if(
      std::begin(res),
      std::end(res),
      [](auto a){return a;}
    );
    const size_t nopes = res.size() - yups;

    if (yups == nopes) return 0;
    else if (yups > nopes) return 1.0 * (yups - nopes) / res.size();
    else return 1.0 * (nopes - yups) / res.size();

  }

  void Reset() {
    matchbin.Clear();
    tags.clear();
    timers.clear();
  }

};
