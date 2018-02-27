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
  emp::vector<Grid<bool>*> gs_taps;
  emp::vector<Grid<int>*> gs_channels;

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
      gs_taps.push_back(new Grid<bool>(GRID_W, GRID_H, false));
      gs_channels.push_back(new Grid<int>(GRID_W, GRID_H, 0));
    }

    // initialize channels
    for (i = 0; i < NLEV; i ++) {
      for(j = 0; j < GRID_A; j ++) {
        gs_channels[i]->Set(j, init_ch(random));
      }
    }

  }
  Simulation & operator=(const Simulation &) = default;

  bool Step() {
    int i;
    for (i = 0; i < NLEV; i ++) {
      event_managers[i].Seed(cupdate, random);
      event_managers[i].UpdateGrid(gs_resources[i], gs_taps[i], cupdate);
    }
    return (++cupdate) < nupdate;
  }

};

#endif
