#pragma once

#include <algorithm>
#include <unordered_map>
#include <tuple>
#include <iostream>

#include "base/vector.h"
#include "base/assert.h"
#include "tools/Random.h"
#include "base/Ptr.h"

#include "GridStack.h"
#include "DishtinyConfig.h"
#include "GridSpec.h"

class ChannelManager {
private:

  GridStack<int> channel;

  GridSpec spec;

  emp::vector<std::unordered_map<int,emp::vector<size_t>>> census;

  emp::Ptr<emp::Random> rand;

  const int CH_MAX;

public:

  constexpr static int DEAD = 0;

  ChannelManager(
    DishtinyConfig& dconfig,
    GridSpec& _spec,
    emp::Ptr<emp::Random> _r)
  : channel(DEAD, _spec, dconfig.NLEV())
  , spec(_spec)
  , census(dconfig.NLEV())
  , rand(_r)
  , CH_MAX(dconfig.CH_MAX()) {
    // initialize channels
    for (size_t lev = 0; lev < channel.GetDepth(); ++lev) {
      for (size_t cell = 0; cell < channel.GetArea(); ++cell) {
        RegisterCh(lev, cell, InitCh());
      }
    }
  }

  inline void Kill(
    size_t cell,
    std::function<void(size_t lev, int ch)> erasepool,
    std::function<void(size_t cell)> erasestockpile
  ) {
    for (size_t lev = 0; lev < channel.GetDepth(); ++lev) {
      KillLev(lev, cell, erasepool, erasestockpile);
    }
  }

  inline int GetChannel(size_t lev, size_t cell) const {
    return channel.Get(lev, cell);
  }

  inline int GetChannel(size_t lev, int x, int y) const {
    return channel.Get(lev, x, y);
  }

  inline size_t GetCensusCount(size_t lev, int ch) {
    return census[lev][ch].size();
  }

  inline size_t GetCensusCell(size_t lev, int ch, size_t memb) {
    return census[lev][ch][memb];
  }

  inline void CopyChannelList(size_t lev, emp::vector<int>& temp) const {
    for (auto it=census[lev].begin(); it != census[lev].end(); ++it) {
      temp.push_back(it->first);
    }
  }

  inline bool ChannelExists(size_t lev, int ch) const {
    return census[lev].find(ch) != census[lev].end();
  }

  inline void Spawn(size_t cell, size_t off_dest, size_t off_level) {
    for (size_t lev = 0; lev < channel.GetDepth(); ++lev) {

      // make sure that off_dest is dead
      emp_assert(channel.Get(lev, off_dest) == DEAD);

      if (lev < off_level) {
        RegisterCh(lev, off_dest, ChangeCh(channel.Get(lev, cell)));
      } else {
        RegisterCh(lev, off_dest, channel.Get(lev, cell));
      }
    }
  }


private:

  // to register a not already living cell
  // be sure to kill the destination cell before using this
  inline void RegisterCh(size_t lev, size_t cell, int ch) {

    census[lev][ch].push_back(cell);

    channel(lev,cell) = ch;

    SortCensusVector(lev, ch);

  }

  inline void KillLev(
    size_t lev,
    size_t cell,
    std::function<void(size_t lev, int ch)> erasepool,
    std::function<void(size_t cell)> erasestockpile
  ) {

    int oldch = channel.Get(lev, cell);

    // if last of particular channel,
    if (census[lev][oldch].size() == 1) {
      // remove resource pool
      erasepool(lev, oldch);
      // remove census list
      census[lev].erase(oldch);

    } else {
      // else just deregister from census
      census[lev][oldch].erase(
        std::remove(census[lev][oldch].begin(),
        census[lev][oldch].end(), cell),
        census[lev][oldch].end()
      );
      // re-sort census vector
      SortCensusVector(lev, oldch);

    }

    // remove stockpile
    erasestockpile(cell);

    // mark cell as dead
    channel(lev, cell) = DEAD;

  }

  struct less_than_key
  {
    const size_t centroid_x;
    const size_t centroid_y;
    const GridSpec& kspec;

    less_than_key(size_t _cx, size_t _cy, GridSpec& _spec)
    : centroid_x(_cx)
    , centroid_y(_cy)
    , kspec(_spec)
    { ; }

    inline bool operator() (size_t idx1, size_t idx2)
    {

      size_t dist1 = kspec.CalcTaxiDist(idx1, centroid_x, centroid_y);
      size_t dist2 = kspec.CalcTaxiDist(idx2, centroid_x, centroid_y);

      return dist1 < dist2;
    }
  };

  inline void SortCensusVector(size_t lev, int ch) {

    emp_assert(census[lev].find(ch) != census[lev].end(), lev, ch);

    // don't need to sort for size two or less
    if (census[lev][ch].size() > 2) {
      // calculate centroid x, y
      size_t cx, cy;
      std::tie(cx, cy) = spec.CalcCentroid(census[lev][ch]);

      // shuffle then sort to ensure random ordering within equivalent orgs
      emp::Shuffle(*rand, census[lev][ch]);
      std::sort(
        census[lev][ch].begin(),
        census[lev][ch].end(),
        less_than_key(cx, cy, spec)
      );
    }

  }


  inline int InitCh() {
    return rand->GetInt(1,CH_MAX);
  }

  inline int ChangeCh(int cur) {
    return InitCh();
  }

};
