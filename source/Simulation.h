#pragma once

#include <iostream>

#include "base/vector.h"
#include "base/Ptr.h"
#include "tools/Random.h"
#include "tools/random_utils.h"
#include "Evolve/World.h"
#include "base/assert.h"

#include "ChannelManager.h"
#include "CustomConfig.h"
#include "DishtinyConfig.h"
#include "Organism.h"
#include "ResourceManager.h"
#include "SignalManager.h"
#include "StoreManager.h"

class Simulation {
private:
  const size_t nupdate;
  size_t cupdate;

  emp::Random rand;

  emp::World<Organism> world;

  GridSpec spec;

  ChannelManager channel;
  ResourceManager resource;
  SignalManager signal;
  StoreManager store;

  const size_t NLEV;
  const size_t GRID_A;
  const double REP_THRESH;

  emp::vector<size_t> shuffler;
  emp::vector<size_t> neighborshuffler;

  size_t birth_loc;

public:
  Simulation(
    int _nupdate,
    int seed,
    DishtinyConfig& dconfig,
    CustomConfig& cconfig)
  : nupdate(_nupdate)
  , cupdate(0)
  , rand(seed)
  , world(rand)
  , spec(dconfig)
  , channel(dconfig, spec, &rand)
  , resource(dconfig, cconfig, spec, &rand)
  , signal(dconfig, spec)
  , store(dconfig, cconfig, spec)
  , NLEV(dconfig.NLEV())
  , GRID_A(dconfig.GRID_H()*dconfig.GRID_W())
  , REP_THRESH(dconfig.REP_THRESH())
  , shuffler(emp::GetPermutation(rand, GRID_A))
  , neighborshuffler(emp::GetPermutation(rand, 4))
  {
    // setup world
    SetupWorld(dconfig, cconfig);
  }

  bool Step() {

    signal.Propagate(channel, world);

    resource.TryReseed(cupdate, signal, channel, world);

    resource.LayResource(cupdate);

    store.Harvest(signal, resource, channel, world);

    store.PayStateCost(signal, resource, channel, world);

    store.SettlePools(channel);

    store.KillDebtors(world);

    // individual reproduction
    emp::Shuffle(rand, shuffler);
    for (size_t i = 0; i < GRID_A; ++i) {
      store.TransactStockpile(
        shuffler[i],
        TryReproduceCell(shuffler[i], store.CheckStockpile(shuffler[i]))
      );
    }

    for (size_t lev = 0; lev < NLEV; ++lev) {

      // need this because reproducing during iteration
      emp::vector<int> temp;
      channel.CopyChannelList(lev, temp);

      for (size_t memb = 0; memb < temp.size(); ++memb) {
        int ch = temp[memb];
        if (channel.ChannelExists(lev, ch)) {
          ReproducePool(lev, ch);
        }
      }

    }

    world.Update();

    std::cout << "update " << cupdate << std::endl;
    return (++cupdate) < nupdate;
  }

  void Steps(size_t len) {
    for (size_t i = 0; i < len; ++i) {
      Step();
    }
    std::cout << "done!" << std::endl;
  }

  void Run() {
    while (Step());
  }

private:
  // need to assess cost outside
  inline void ReproduceCell(size_t parent, size_t dest, size_t off_level, double endowment) {

    birth_loc = dest;

    Organism *child = new Organism(world.GetOrg(parent));
    child->DoMutations(rand);

    // takes care of killing trampled cell
    world.DoBirth(*child, parent);
    world.AddOrgAt(child, dest);

    channel.Spawn(parent, dest, off_level);

    // give endowment
    store.MultilevelTransaction(world.GetOrg(dest), dest, channel, endowment);

  }

  inline int pick_off_level(size_t cell, double avail_resource) {

    emp_assert(world.IsOccupied(cell));

    const Organism& org = world.GetOrg(cell);

    int rlev = -1;

    // check endowments and caps
    for (size_t tlev = 0; tlev < NLEV + 1; ++tlev) {
      bool res = true;

      // check cap
      for (size_t i = tlev; i < NLEV; ++i) {
        res &= (channel.GetCensusCount(i, channel.GetChannel(i, cell)) < org.GetOffChCap(i));
      }

      // check endowment
      res &= (avail_resource >= org.GetEndowment(tlev) + REP_THRESH);


      if (res) {
        rlev = tlev;
        break;
      }
    }

    return rlev;
  }

  inline int pick_off_dest(size_t cell) {
    bool viable[4];
    for (size_t n = 0; n < 4; ++n) {
      viable[n] = true;
    }

    const size_t x = spec.GetX(cell);
    const size_t y = spec.GetY(cell);

    const Organism& org = world.GetOrg(cell);

    for (size_t lev = 0; lev < NLEV; ++lev) {
      if (org.GetOffOver(lev) < rand.GetDouble()) {
        int cur_ch = channel.GetChannel(lev, cell);

        viable[0] &= (channel.GetChannel(lev,x,y+1) != cur_ch);
        viable[1] &= (channel.GetChannel(lev,x,y-1) != cur_ch);
        viable[2] &= (channel.GetChannel(lev,x+1,y) != cur_ch);
        viable[3] &= (channel.GetChannel(lev,x-1,y) != cur_ch);
      }
    }

    emp::Shuffle(rand, neighborshuffler);
    for (size_t n = 0; n < 4; ++n) {
      if(viable[neighborshuffler[n]]) {
        switch(neighborshuffler[n]) {
          case 0 :
            return spec.GetID(x,y+1);
          case 1 :
            return spec.GetID(x,y-1);
          case 2 :
            return spec.GetID(x+1,y);
          case 3 :
            return spec.GetID(x-1,y);;
        }
      }

    }

    return -1;
  }

  inline double TryReproduceCell(size_t cell, double avail_resource) {
    if (!world.IsOccupied(cell)) {
      return 0.0;
    }

    int off_dest = pick_off_dest(cell);
    if (off_dest == -1) {
      return 0.0;
    }

    int off_level = pick_off_level(cell, avail_resource);
    if (off_level == -1) {
      return 0.0;
    }

    double endow = world.GetOrg(cell).GetEndowment(off_level);

    ReproduceCell(cell, off_dest, off_level, endow);

    return -REP_THRESH - endow;
  }


  inline void ReproducePool(size_t lev, int ch) {

    double res;
    do {

      res = 0.0;
      for (
        size_t memb = 0;
        (memb < channel.GetCensusCount(lev, ch) &&
        res == 0.0 &&
        store.CheckPool(lev,ch) >= REP_THRESH);
        ++memb
        ) {

          res = TryReproduceCell(
            channel.GetCensusCell(lev,ch,memb),
            store.CheckPool(lev,ch)
            );
          store.TransactPool(lev, ch, res);

      }
    } while (store.CheckPool(lev, ch) >= REP_THRESH && channel.GetCensusCount(lev,ch) > 0 && res != 0.0);

  }

  inline void SetupWorld(DishtinyConfig& dconfig, CustomConfig& cconfig) {
    world.SetGrid(dconfig.GRID_W(), dconfig.GRID_H());

    world.OnOrgDeath( [this](size_t pos) {
      channel.Kill(
        pos,
        [this](size_t lev, int ch){ store.ErasePool(lev, ch); },
        [this](size_t cell){ store.EraseStockpile(cell); }
      );
      store.EraseStockpile(pos);
    } );

    world.SetAddBirthFun(
      [this](emp::Ptr<Organism> new_org, size_t parent_id) {
      return birth_loc;
    } );

    // populate the world
    for (size_t cell = 0; cell < GRID_A; ++cell) {
      world.InjectAt(*(new Organism(&rand, dconfig, &cconfig)), cell);
    }

  }

};
