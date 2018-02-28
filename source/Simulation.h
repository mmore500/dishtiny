#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "base/vector.h"
#include "tools/Random.h"
#include "tools/random_utils.h"

#include "EventManager.h"
#include "Grid.h"
#include "param.h"

inline size_t count_foreigns(Grid<int> *g_channels, size_t cell) {
  size_t count = 0;
  int cur_ch = (*g_channels)(cell);
  size_t x = g_channels->GetX(cell);
  size_t y = g_channels->GetY(cell);
  if ((*g_channels)(x,y+1) != cur_ch) {
    count ++;
  }
  if ((*g_channels)(x,y-1) != cur_ch) {
    count ++;
  }
  if ((*g_channels)(x+1,y) != cur_ch) {
    count ++;
  }
  if ((*g_channels)(x-1,y) != cur_ch) {
    count ++;
  }

  return count;
}


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
  emp::vector<std::unordered_map<int,emp::vector<size_t>>> counts;


private:
  emp::Random random;
  emp::vector<size_t> shuffler;
  emp::vector<size_t> neighborshuffler;

public:
  Simulation(int _nupdate, int _seed)
  : nupdate(_nupdate)
  , cupdate(0)
  , random(_seed)
  , shuffler(emp::GetPermutation(random, GRID_A))
  , neighborshuffler(emp::GetPermutation(random, 4)) {

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
      counts.push_back(*(new std::unordered_map<int,emp::vector<size_t>>));
    }

    // initialize channels, stockpiles
    for (i = 0; i < NLEV; i ++) {
      for(j = 0; j < GRID_A; j ++) {
        int newch = init_ch(random);
        gs_channels[i]->Set(j, newch);
        counts[i][newch].push_back(j);
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

    counts[l][oldch].erase(std::remove( counts[l][oldch].begin(),   counts[l][oldch].end(), dest), counts[l][oldch].end());
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
      pools[i][(*gs_channels[i])(cell)] += amt * (*gs_res_pools[i+1])(cell);
    }
  }

  // need to assess cost outside
  void inline reproduce(size_t parent, size_t dest, int newchannel[NLEV], double endowment) {
    bool balance_res_pool = false;

    kill_cell(dest);

    for (size_t l = 0; l < NLEV; l ++) {

      counts[l][newchannel[l]].push_back(dest);

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
    gs_res_pools[NLEV]->Set(dest, off_val);

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
        for (std::vector<size_t>::iterator jt=counts[lev][it->first].begin(); jt != counts[lev][it->first].end(); ++jt) {
          (*g_stockpiles).Incr(*jt, amt);
        }
        it->second = 0;
      }
    }

  }

  int pick_off_level(size_t cell, double avail_resource) {

    int rlev = -1;

    // check endowments and caps
    for (size_t tlev = 0; tlev < NLEV + 1; tlev ++) {
      bool res = true;
      // check cap
      for (size_t i = tlev; i < NLEV; i ++) {
        res &= !(
          counts[i][(*gs_channels[i])(cell)].size() >= (*gs_off_ch_caps[i])(cell)
        );
      }

      // check endowment
      res &= (avail_resource >= (*gs_endowments[tlev])(cell) + REP_THRESH);


      if (res) {
        rlev = tlev;
        break;
      }
    }

    return rlev;
  }

  inline int pick_off_dest(size_t cell) {
    bool viable[4];
    for (size_t n = 0; n < 4; n ++) {
      viable[n] = true;
    }

    size_t x = g_stockpiles->GetX(cell);
    size_t y = g_stockpiles->GetY(cell);


    for (size_t l = 0; l < NLEV; l ++) {
      if ((*gs_off_overs[l])(cell) < random.GetDouble()) {
        int cur_ch = (*gs_channels[l])(cell);

        viable[0] &= ((*gs_channels[l])(x,y+1) != cur_ch);
        viable[1] &= ((*gs_channels[l])(x,y-1) != cur_ch);
        viable[2] &= ((*gs_channels[l])(x+1,y) != cur_ch);
        viable[3] &= ((*gs_channels[l])(x-1,y) != cur_ch);
      }
    }

    emp::Shuffle(random, neighborshuffler);
    for (size_t n = 0; n < 4; n ++) {
      if(viable[neighborshuffler[n]]) {
        switch(neighborshuffler[n]) {
          case 0 :
            return g_stockpiles->GetID(x,y+1);
          case 1 :
            return g_stockpiles->GetID(x,y-1);
          case 2 :
            return g_stockpiles->GetID(x+1,y);
          case 3 :
            return g_stockpiles->GetID(x-1,y);;
        }
      }

    }

    return -1;
  }

  inline double try_repr(size_t cell, double avail_resource) {
    if ((*gs_channels[0])(cell) == DEAD) {
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

    double endow = (*gs_endowments[off_level])[cell];

    int newchannel[NLEV];

    for ( int l = 0; l < NLEV; l ++ ) {
      if (l < off_level) {
        newchannel[l] = change_ch((*gs_channels[l])[cell], random);
      } else {
        newchannel[l] = (*gs_channels[l])[cell];
      }
    }

    reproduce(cell, off_dest, newchannel, endow);

    return -REP_THRESH - endow;
  }


  struct less_than_key
  {
    Grid<int> *g_channels;

    less_than_key(Grid<int> *_g_channels) : g_channels(_g_channels) { ; }

    inline bool operator() (size_t idx1, size_t idx2)
    {
        return count_foreigns(g_channels, idx1) < count_foreigns(g_channels, idx2);
    }
  };

  inline void repr_pool(int ch, size_t lev) {

    emp::Shuffle(random, counts[lev][ch]);
    std::sort(counts[lev][ch].begin(), counts[lev][ch].end(), less_than_key(gs_channels[lev]));

    for ( size_t i = 0; i < counts[lev][ch].size() && pools[lev][ch] >= REP_THRESH; i ++ ) {

      double res;
      do {
        res = try_repr(counts[lev][ch][i], pools[lev][ch]);
        pools[lev][ch] += res;
      } while (res != 0.0) ;
    }

  }


  bool Step() {

    for (size_t l = 0; l < NLEV; l ++) {

      propagate_signals(l);

      // seed new events, perform taps, update resource grid
      event_managers[l].Seed(cupdate, random);
      event_managers[l].UpdateGrid(gs_resources[l], gs_signals[l], gs_channels[l], cupdate);

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

    for (size_t i = 0; i < GRID_A; i ++) {
      if ((*g_stockpiles)(i) <= KILL_THRESH) kill_cell(i);
    }

    for (size_t c = 0; c < GRID_A; c ++) {
      g_stockpiles->Incr(c, try_repr(c, (*g_stockpiles)(c)));
    }

    for (size_t lev = 0; lev < NLEV; lev ++) {
      for (std::unordered_map<int,double>::iterator it=pools[lev].begin(); it != pools[lev].end(); ++it) {
        repr_pool(it->first, lev);
      }
    }

    std::cout << "update " << cupdate << std::endl;
    return (++cupdate) < nupdate;
  }

  void Steps(int len) {
    for (int c = 0; c < len; c ++) {
      Step();
    }
    std::cout << "done!" << std::endl;
  }

};

#endif
