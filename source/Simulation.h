#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>

#include "base/vector.h"
#include "tools/Random.h"

#include "EventManager.h"
#include "Grid.h"
#include "param.h"

class Simulation {
public:
  int nupdate;
  int cupdate;
  emp::vector<EventManager> event_managers;
  emp::vector<Grid<double>*> gs_resources;
  emp::vector<Grid<double>*> gs_stockpiles;
  emp::vector<Grid<int>*> gs_channels;
  emp::vector<Grid<int>*> gs_signals;
  emp::vector<Grid<int>*> gs_sigbuffs;

  emp::vector<Grid<double>*> gs_endowments;
  emp::vector<Grid<double>*> gs_res_pools;
  emp::vector<Grid<double>*> gs_off_overs;
  emp::vector<Grid<double>*> gs_off_ch_caps;


private:
  emp::Random random;

public:
  Simulation(int _nupdate, int _seed)
  : nupdate(_nupdate)
  , cupdate(0)
  , random(_seed) {
    int i, j;
    // build grids
    for (i = 0; i < NLEV; i ++) {
      event_managers.push_back(EventManager(i));
      gs_resources.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_stockpiles.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_channels.push_back(new Grid<int>(GRID_W, GRID_H, 0));
      gs_signals.push_back(new Grid<int>(GRID_W, GRID_H, READY));
      gs_sigbuffs.push_back(new Grid<int>(GRID_W, GRID_H, READY));

      gs_endowments.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_res_pools.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_off_overs.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_off_ch_caps.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));

    }


    // initialize channels, stockpiles
    for (i = 0; i < NLEV; i ++) {
      for(j = 0; j < GRID_A; j ++) {
        gs_channels[i]->Set(j, init_ch(random));
        gs_stockpiles[i]->Set(j, init_stockpile(random));
        gs_off_ch_caps[i]->Set(j, init_off_ch_cap(i, random));
        gs_endowments[i]->Set(j, init_endowment(i, random));
        gs_res_pools[i]->Set(j, init_res_pool(i, random));
        gs_off_overs[i]->Set(j, init_off_over(i, random));
      }
    }

    // there are one more required for these genotype characters
    gs_endowments.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
    gs_res_pools.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
    for(j = 0; j < GRID_A; j ++) {
      gs_endowments[NLEV]->Set(j, init_endowment(NLEV, random));
      gs_res_pools[NLEV]->Set(j, init_res_pool(NLEV, random));
    }


  }
  Simulation & operator=(const Simulation &) = default;

  void inline reproduce(size_t parent, size_t dest, int newchannel[NLEV], double endowment) {
    bool balance_res_pool = false;
    for (size_t l = 0; l < NLEV; l ++) {
      gs_channels[l]->Set(dest, newchannel[l]);
      gs_stockpiles[l]->Incr(parent, -REP_THRESH -endowment);
      gs_stockpiles[l]->Set(dest, endowment);

      gs_endowments[l]->Set(dest, mut_endowment((*gs_endowments[l])(parent), l, random));

      double par_val = (*gs_res_pools[l])(parent);
      double off_val = mut_res_pool(par_val, l, random);
      balance_res_pool |= (par_val != off_val);
      gs_res_pools[l]->Set(dest, off_val);
      gs_off_overs[l]->Set(dest, mut_off_over((*gs_off_overs[l])(parent), l, random));
      gs_off_ch_caps[l]->Set(dest, mut_off_ch_cap((*gs_off_ch_caps[l])(parent), l, random));
    }

    gs_endowments[NLEV]->Set(dest, mut_endowment((*gs_endowments[NLEV])(parent), NLEV, random));

    double par_val = (*gs_res_pools[NLEV])(parent);
    double off_val = mut_res_pool(par_val, NLEV, random);
    balance_res_pool |= (par_val != off_val);
    gs_res_pools[NLEV]->Set(dest, mut_res_pool(off_val, NLEV, random));

    // balance res_pool
    if (balance_res_pool) {
      double vals[NLEV + 1];
      for (size_t l = 0; l < NLEV + 1; l ++) {
        vals[l] = (*gs_res_pools[l])(dest);
      }

      bal_res_pool(vals);
      for (size_t l = 0; l < NLEV + 1; l ++) {
        gs_res_pools[l]->Set(dest, vals[l]);
      }
    }
  }

  bool Step() {
    for (int l = 0; l < NLEV; l ++) {

      gs_sigbuffs[l]->reset();

      for (int i = 0; i < GRID_A; i ++) {
        int sigstate = (*gs_signals[l])(i);
        if (sigstate > 0) gs_sigbuffs[l]->Set(i,sigstate-1);
        else if (sigstate == ACTIVATED) {
          gs_sigbuffs[l]->Set(i, QUIESCENT_MAX);
          size_t x = gs_signals[l]->GetX(i);
          size_t y = gs_signals[l]->GetY(i);
          int cur_ch = (*gs_channels[l])(i);
          if ((*gs_signals[l])(x,y+1) == READY && (*gs_channels[l])(x,y+1) == cur_ch) {
            gs_sigbuffs[l]->Set(x,y+1,ACTIVATED);
          }
          if ((*gs_signals[l])(x,y-1) == READY && (*gs_channels[l])(x,y-1) == cur_ch) {
            gs_sigbuffs[l]->Set(x,y-1,ACTIVATED);
          }
          if ((*gs_signals[l])(x+1,y) == READY && (*gs_channels[l])(x+1,y) == cur_ch) {
            gs_sigbuffs[l]->Set(x+1,y,ACTIVATED);
          }
          if ((*gs_signals[l])(x-1,y) == READY && (*gs_channels[l])(x-1,y) == cur_ch) {
            gs_sigbuffs[l]->Set(x-1,y,ACTIVATED);
          }
        }
      }

      // swap buffer in
      Grid<int> *temp = gs_sigbuffs[l];
      gs_sigbuffs[l] = gs_signals[l];
      gs_signals[l] = temp;

      // seed new events, perform taps, update resource grid
      event_managers[l].Seed(cupdate, random);
      event_managers[l].UpdateGrid(gs_resources[l], gs_signals[l], cupdate);

      // distribute resources to activated cells
      for (int i = 0; i < GRID_A; i ++) {
        int sigstate = (*gs_signals[l])(i);
        if (sigstate == ACTIVATED) {
          gs_stockpiles[l]->Incr(i, (*gs_resources[l])(i));
          gs_stockpiles[l]->Incr(i, V_COST[l]);
        }
      }

    }

    return (++cupdate) < nupdate;
  }

};

#endif
