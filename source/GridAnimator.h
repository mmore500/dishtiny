#ifndef GRIDANIMATOR_H
#define GRIDANIMATOR_H

#include "web/Canvas.h"
#include "web/Animate.h"

#include "Simulation.h"
#include "Grid.h"

template <typename STATE_TYPE>
class GridAnimator {
public:

  Simulation simulation;
  Grid<STATE_TYPE> *grid;
  const STATE_TYPE *minp;
  const STATE_TYPE *maxp;
  emp::web::Canvas canvas;
  emp::web::Animate anim;


public:
  GridAnimator(
    Simulation _s,
    Grid<STATE_TYPE> *_g,
    STATE_TYPE *_minp,
    STATE_TYPE *_maxp,
    emp::web::Canvas _c)
    : simulation(_s)
    , grid(_g)
    , minp(_minp)
    , maxp(_maxp)
    , canvas(_c)
    , anim([this](){ DoFrame(); }) { ; }

  void DoFrame() {
    simulation.Step();
    grid->Draw(canvas, minp, maxp);
  }

};

#endif
