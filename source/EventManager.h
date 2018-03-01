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

  const size_t lev;
  std::list<Event> lis;

public:
  EventManager(size_t _lev) : lev(_lev)
  { ; }

  void inline Seed(size_t t, emp::Random& r) {
    if (t % EVENT_RADII[lev] == 0) {
        size_t bx = r.GetUInt(0,GRID_W);
        size_t by = r.GetUInt(0,GRID_H);
        for (size_t ix = 0; ix <= GRID_W - 2* EVENT_RADII[lev]; ix += (2 * EVENT_RADII[lev])) {
          for (size_t iy = 0; iy <= GRID_H - 2 *EVENT_RADII[lev]; iy += (2 *EVENT_RADII[lev])) {
            lis.push_back(Event(ix + bx, iy + by, t, EVENT_RADII[lev], EVENT_VALS[lev]));
          }
        }
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


};

#endif
