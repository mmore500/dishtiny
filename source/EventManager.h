#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <list>
#include <iostream>

#include "base/vector.h"

#include "Grid.h"
#include "macro.h"
#include "param.h"

class Event {
public:

  const int x;
  const int y;
  const int timestamp;
  const int duration;
  const double val;

public:
  Event(int _x, int _y, int _timestamp, int _duration, double _val)
    : x(_x)
    , y(_y)
    , timestamp(_timestamp)
    , duration(_duration)
    , val(_val) {;}

    ~Event() = default;

  bool PlaceOnGrid(Grid<double> *resource_grid, Grid<int> *signal_grid, Grid<int> *channel_grid, int curtime) const {
    // return -1 if duration exceeded
    bool rval;
    int elapsetime = curtime - timestamp;
    if (elapsetime >= duration) {
      rval = false;
    } else if (elapsetime == 0) {
      resource_grid->Incr(x,y,val);
      if ((*signal_grid)(x,y) == READY && (*channel_grid)(x,y) != DEAD) signal_grid->Set(x,y,ACTIVATED);
      rval = true;
    } else {
      int i;
      for (i = 0; i < elapsetime; i ++) {
        resource_grid->Incr(x+i, y+elapsetime-i, val);
        resource_grid->Incr(x+elapsetime-i, y-i, val);
        resource_grid->Incr(x-i, y-elapsetime+i, val);
        resource_grid->Incr(x-elapsetime+i, y+i, val);
      }
      rval = true;
    }
    return rval;
  }

};

class EventManager {
private:

  const int lev;
  const int lev_size;
  std::list<Event> lis;

public:
  EventManager(int _lev) : lev(_lev), lev_size(EVENT_VALS_LENS[lev])
  { ; }

  void inline Seed(int t, emp::Random& r) {
    for (int i = 0; i < lev_size; i ++) {
      SeedEvent(i, t, r);
    }
  }

  void UpdateGrid(Grid<double> *resource_grid, Grid<int> *signal_grid, Grid<int> *channel_grid, int curtime) {
    resource_grid->reset();

    std::list<Event>::iterator it = lis.begin();
    while (it != lis.end()) {
      if ((*it).PlaceOnGrid(resource_grid, signal_grid, channel_grid, curtime)) {
        ++it;
      } else {
        it = lis.erase(it);
      }
    }

  }

private:
  void inline SeedEvent(int event, int t, emp::Random& r) {
    uint32_t n = r.GetRandBinomial(GRID_A, EVENT_PROBS[lev][event]);
    for (uint32_t i = 0; i < n; i ++) {
      int ex = r.GetInt(GRID_W);
      int ey = r.GetInt(GRID_H);
      lis.push_back(Event(ex, ey, t, EVENT_RADII[lev][event], EVENT_VALS[lev][event]));
    }
  }

};

#endif
