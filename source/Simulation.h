#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>

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
  Grid<double>* g_stockpiles;
  emp::vector<Grid<int>*> gs_channels;
  emp::vector<Grid<int>*> gs_signals;
  emp::vector<Grid<int>*> gs_sigbuffs;

  emp::vector<Grid<double>*> gs_endowments;
  emp::vector<Grid<double>*> gs_res_pools;
  emp::vector<Grid<double>*> gs_off_overs;
  emp::vector<Grid<double>*> gs_off_ch_caps;

  emp::vector<std::unordered_map<int,double>> pools;
  emp::vector<std::unordered_map<int,std::unordered_set<size_t>>> counts;


private:
  emp::Random random;

public:
  Simulation(int _nupdate, int _seed)
  : nupdate(_nupdate)
  , cupdate(0)
  , random(_seed) {

    g_stockpiles = new Grid<double>(GRID_W, GRID_H, 0.0);


    int i, j;
    // build grids and maps
    for (i = 0; i < NLEV; i ++) {
      event_managers.push_back(EventManager(i));
      gs_resources.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_channels.push_back(new Grid<int>(GRID_W, GRID_H, 0));
      gs_signals.push_back(new Grid<int>(GRID_W, GRID_H, READY));
      gs_sigbuffs.push_back(new Grid<int>(GRID_W, GRID_H, READY));

      gs_endowments.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_res_pools.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_off_overs.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));
      gs_off_ch_caps.push_back(new Grid<double>(GRID_W, GRID_H, 0.0));

      pools.push_back(*(new std::unordered_map<int,double>));
      counts.push_back(*(new std::unordered_map<int,std::unordered_set<size_t>>));
    }

    // initialize channels, stockpiles
    for (i = 0; i < NLEV; i ++) {
      for(j = 0; j < GRID_A; j ++) {
        int newch = init_ch(random);
        gs_channels[i]->Set(j, newch);
        counts[i][newch].insert(j);
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
      g_stockpiles->Set(j, init_stockpile(random));
    }


  }
  Simulation & operator=(const Simulation &) = default;

  void inline kill_cell(size_t dest) {
    for (size_t l = 0; l < NLEV; l ++) {
      int oldch = (*gs_channels[l])(dest);

      counts[l][oldch].erase(dest);
      if(counts[l][oldch].size() == 0) {
        pools[l].erase(oldch);
        counts[l].erase(oldch);
      }

      gs_channels[l]->Set(dest, 0);
      gs_endowments[l]->Set(dest, -1.0);
      gs_res_pools[l]->Set(dest, -1.0);
      gs_off_overs[l]->Set(dest, -1.0);
      gs_off_ch_caps[l]->Set(dest, -1.0);

    }

    g_stockpiles->Set(dest, 0.0);
    gs_endowments[NLEV]->Set(dest, -1.0);
    gs_res_pools[NLEV]->Set(dest, -1.0);

  }


  void inline multilevel_transaction(size_t cell, double amt) {
    g_stockpiles->Incr(cell, amt * (*gs_res_pools[0])[cell]);
    for (size_t i = 0; i < NLEV; i ++) {
      pools[i][(*gs_channels[cell])(cell)] += amt * (*gs_res_pools[i+1])(cell);
    }
  }

  // need to assess cost outside
  void inline reproduce(size_t parent, size_t dest, int newchannel[NLEV], double endowment) {
    bool balance_res_pool = false;
    for (size_t l = 0; l < NLEV; l ++) {

      kill_cell(dest);
      counts[l][newchannel[l]].insert(dest);

      gs_channels[l]->Set(dest, newchannel[l]);

      gs_endowments[l]->Set(dest, mut_endowment((*gs_endowments[l])(parent), l, random));

      double par_val = (*gs_res_pools[l])(parent);
      double off_val = mut_res_pool(par_val, l, random);
      balance_res_pool |= (par_val != off_val);
      gs_res_pools[l]->Set(dest, off_val);
      gs_off_overs[l]->Set(dest, mut_off_over((*gs_off_overs[l])(parent), l, random));
      gs_off_ch_caps[l]->Set(dest, mut_off_ch_cap((*gs_off_ch_caps[l])(parent), l, random));
    }

    // give endowment (multilivel split like res pool)
    g_stockpiles->Set(dest, endowment * (*gs_res_pools[0])[parent]);
    for (size_t i = 0; i < NLEV; i ++) {
      pools[i][newchannel[i]] += endowment * (*gs_res_pools[i+1])(parent);
    }

    // mutated inheritance
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

  inline void propagate_signals(size_t l) {

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

  }

  // redistribute negative pool values to individuals
  inline void harm_pool(size_t lev) {
    for (std::unordered_map<int,double>::iterator it=pools[lev].begin(); it != pools[lev].end(); ++it) {
      if (it->second < 0) {
        double amt = it->second / counts[lev][it->first].size();
        for (std::unordered_set<size_t>::iterator jt=counts[lev][it->first].begin(); jt != counts[lev][it->first].end(); ++jt) {
          (*g_stockpiles).Incr(*jt, amt);
        }
        it->second = 0;
      }
    }

  }

  inline void kill() {
    for (size_t i = 0; i < GRID_A; i ++) {
      if ((*g_stockpiles)(i) <= KILL_THRESH) kill_cell(i);
    }
  }

  bool Step() {

    for (size_t l = 0; l < NLEV; l ++) {

      propagate_signals(l);

      // seed new events, perform taps, update resource grid
      event_managers[l].Seed(cupdate, random);
      event_managers[l].UpdateGrid(gs_resources[l], gs_signals[l], cupdate);

      // distribute resources to activated cells
      for (size_t i = 0; i < GRID_A; i ++) {
        int sigstate = (*gs_signals[l])(i);
        if (sigstate == ACTIVATED) {
          multilevel_transaction(i, (*gs_resources[l])(i) + V_COST[l]);
        }
      }


    }
    for (size_t l = 0; l < NLEV; l ++) {
      harm_pool(l);
    }

    kill();

    return (++cupdate) < nupdate;
  }

};

#endif
