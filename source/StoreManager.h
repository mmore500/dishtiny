#pragma once

#include <unordered_map>

#include "Evolve/World.h"
#include "base/vector.h"
#include "base/assert.h"
#include "tools/Random.h"

#include "Grid.h"
#include "Organism.h"
#include "ChannelManager.h"
#include "ResourceManager.h"
#include "SignalManager.h"

class StoreManager {
private:
  // each individual cell's resource stockpile
  Grid<double> stockpile;
  // channel resource pools organized by level then by channel
  emp::vector<std::unordered_map<int,double>> pool;
  CustomConfig cconfig;
  const size_t NLEV;
  const double REP_THRESH;
  const double KILL_THRESH;

public:
  StoreManager(
    DishtinyConfig& dconfig,
    CustomConfig& _cconfig,
    GridSpec& spec)
  : stockpile(0.0, spec)
  , pool(dconfig.NLEV())
  , cconfig(_cconfig)
  , NLEV(dconfig.NLEV())
  , REP_THRESH(dconfig.REP_THRESH())
  , KILL_THRESH(dconfig.KILL_THRESH())
  {

    // initialize stockpiles with some resource
    for (size_t cell = 0; cell < stockpile.GetArea(); cell ++)
    {
      stockpile(cell) = init_stockpile();
    }

  }

  /*
   * Erase a pool -- resets resource accumulation and clears up memory.
   */
  inline void ErasePool(size_t lev, int ch) {
    pool[lev].erase(ch);
  }

  /*
   * Set an individual's stockpile to zero.
   */
  inline void EraseStockpile(size_t cell) {
    stockpile(cell) = 0.0;
  }

  /*
   * Pay a cost or accrue a benefit of amount amt split over a cell's channel
   * resource pools and own stockpile. The split distribution is determined by
   * the organism's genotype.
   */
  inline void MultilevelTransaction(
    Organism& org,
    size_t cell,
    ChannelManager& cm,
    double amt
  ) {
    // how much should be paid out of own pocket?
    stockpile(cell) += amt * org.GetResPool(0);

    // how much should be paid out of pool resources?
    for (size_t lev = 0; lev < NLEV; ++lev) {
      pool[lev][cm.GetChannel(lev, cell)] += amt * org.GetResPool(lev+1);
    }
  }

  /*
   * Redistribute negative pool values to individual stockpiles.
   */
  inline void SettlePools(ChannelManager& cm) {
    for (size_t lev = 0; lev < pool.size(); ++lev) {
      for (auto pl=pool[lev].begin(); pl != pool[lev].end(); ++pl) {
        // pl->first is pool channel
        // pl->second is pool amount

        // only need to settle if pool is negative
        if (pl->second < 0) {

          double amt_each = pl->second / cm.GetCensusCount(lev, pl->first);

          // iterate over channel members
          for (size_t m = 0; m < cm.GetCensusCount(lev, pl->first); ++m) {
            // distribute to each agent its share of the negative pool value
            stockpile(cm.GetCensusCell(lev, pl->first, m)) += amt_each;
          }
          // all negative value in the pool has been redistributed
          pl->second = 0;

        }
      }
    }
  }

  /*
   * Accessor function.
   */
  inline double CheckStockpile(size_t cell) const {
    return stockpile.Get(cell);
  }

  /*
   * Accessor function.
   */
  inline double CheckPool(size_t lev, int ch) {
    return pool[lev][ch];
  }

  /*
   * Perform a transaction with a cell's stockpile.
   */
  inline void TransactStockpile(size_t cell, double amt) {
    stockpile(cell) += amt;
  }

  /*
   * Perform a transaction with a channel resource pool.
   */
  inline void TransactPool(size_t lev, int ch, double amt) {
    pool[lev][ch] += amt;
  }

  /*
   * If cell in ACVTIVE state while resouce wave passing over, harvest the
   * resource. Resource is stored across stockpile, channel pools through
   * MultilevelTransaction.
   */
  inline void Harvest(
    SignalManager& s,
    ResourceManager& r,
    ChannelManager& c,
    emp::World<Organism>& w
    ) {
    for (size_t lev = 0; lev < pool.size(); ++lev) {
      HarvestLev(lev, s, r, c, w);
    }
  }

  /*
   * If cell is in ACTIVE state, pay cost from stockpile, channel pools through
   * MultilevelTransaction.
   */
  inline void PayStateCost(
    SignalManager &s,
    ResourceManager &r,
    ChannelManager &c,
    emp::World<Organism>& w
    ) {
    for (size_t lev = 0; lev < pool.size(); ++lev) {
      PayStateCostLev(lev, s, r, c, w);
    }
  }

  /*
   * Kill cells with stockpile value at or below KILL_THRESH.
   */
  inline void KillDebtors(emp::World<Organism>& w) {
    for (size_t cell = 0; cell < stockpile.GetArea(); ++cell) {
      if (CheckStockpile(cell) <= KILL_THRESH) {
        w.DoDeath(cell);
      }
    }
  }

private:

  /*
   * Initialize stockpile at beginning of simulation.
   */
  inline double init_stockpile() const {
    return REP_THRESH - 0.1;
  }

  /*
   * If cell is in ACTIVE state, pay cost from stockpile, channel pools through
   * MultilevelTransaction.
   */
  inline void PayStateCostLev(
    size_t lev,
    SignalManager& s,
    ResourceManager& r,
    ChannelManager& c,
    emp::World<Organism>& w
    ) {
      // make live cells pay a cost for being vulnerable
      for (size_t cell = 0; cell < stockpile.GetArea(); ++cell) {
        if (w.IsOccupied(cell)) {
        if (s.IsActivated(lev, cell)) {
          MultilevelTransaction(
            w.GetOrg(cell),
            cell,
            c,
            cconfig.V_COST[lev]
          );
        }
        }
      }
  }

  /*
   * If cell is in ACVTIVE state while resouce wave passing over, harvest the
   * resource. Resource is stored across stockpile, channel pools through
   * MultilevelTransaction.
   */
  inline void HarvestLev(
    size_t lev,
    SignalManager& s,
    ResourceManager& r,
    ChannelManager& c,
    emp::World<Organism>& w
    ) {
    // distribute resources to activated cells
    for (size_t cell = 0; cell < stockpile.GetArea(); ++cell) {
      if (s.IsActivated(lev, cell) && w.IsOccupied(cell)) {
        MultilevelTransaction(
          w.GetOrg(cell),
          cell,
          c,
          r.CheckResource(lev, cell)
        );
      }
    }
  }

};
