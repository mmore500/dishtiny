#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <list>
#include <iostream>

#include "base/vector.h"

#include "Grid.h"
#include "macro.h"
#include "param.h"

class EventManager {
private:

  const size_t lev;
  size_t curx;
  size_t cury;
  size_t startt;

public:
  EventManager(size_t _lev)
  : lev(_lev)
  , curx(0)
  , cury(0)
  , startt(0)
  { ; }

  void inline Seed(size_t t, Grid<int> *signal_grid, Grid<int> *channel_grid, emp::Random& r) {
    if (t % EVENT_RADII[lev] == 0) {
        curx = r.GetUInt(0,GRID_W);
        cury = r.GetUInt(0,GRID_H);
        startt = t;

    for (int ix = 0; ix <= GRID_W - 2*EVENT_RADII[lev]; ix += (2 * EVENT_RADII[lev])) {
      for (int iy = 0; iy <= GRID_H - 2*EVENT_RADII[lev]; iy += (2 * EVENT_RADII[lev])) {

       if ((*signal_grid)(curx + ix,cury + iy) == READY && (*channel_grid)(curx + ix, cury + iy) != DEAD) signal_grid->Set(curx + ix, cury + iy, ACTIVATED);

      }
    }

    }
  }

  void UpdateGrid(Grid<double> *resource_grid, int curtime) {
    resource_grid->reset();

    int elapsetime = curtime - startt;
    for (int ix = 0; ix <= GRID_W - 2*EVENT_RADII[lev]; ix += (2 * EVENT_RADII[lev])) {
      for (int iy = 0; iy <= GRID_H - 2*EVENT_RADII[lev]; iy += (2 * EVENT_RADII[lev])) {
          if (elapsetime == 0) {
            resource_grid->Incr(curx+ix, cury+iy, EVENT_VALS[lev]);
          } else {
            for (int i = 0; i < elapsetime; i ++) {
              resource_grid->Incr(curx+ix+i, cury+iy+elapsetime-i, EVENT_VALS[lev]);
              resource_grid->Incr(curx+ix+elapsetime-i, cury+iy-i, EVENT_VALS[lev]);
              resource_grid->Incr(curx+ix-i, cury+iy-elapsetime+i, EVENT_VALS[lev]);
              resource_grid->Incr(curx+ix-elapsetime+i, cury+iy+i, EVENT_VALS[lev]);
            }
          }
        }
      }
    }

};

#endif
