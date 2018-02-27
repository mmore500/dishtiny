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
    }

    // initialize channels, stockpiles
    for (i = 0; i < NLEV; i ++) {
      for(j = 0; j < GRID_A; j ++) {
        gs_channels[i]->Set(j, init_ch(random));
        gs_stockpiles[i]->Set(j, init_stockpile(random));
      }
    }

  }
  Simulation & operator=(const Simulation &) = default;

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
