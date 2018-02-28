#ifndef GRIDANIMATOR_H
#define GRIDANIMATOR_H

#include "web/Canvas.h"
#include "web/Animate.h"

#include "Simulation.h"
#include "Grid.h"

template <typename STATE_TYPE>
class GridAnimator {
public:

  Simulation *simulation;
  Grid<STATE_TYPE> *grid;
  const STATE_TYPE *minp;
  const STATE_TYPE *maxp;
  emp::web::Canvas *canvas;
  emp::vector<std::function<void()>> *cbs;
  emp::web::Animate anim;

public:
  GridAnimator(
    Simulation *_s,
    Grid<STATE_TYPE> *_g,
    STATE_TYPE *_minp,
    STATE_TYPE *_maxp,
    emp::web::Canvas *_c,
    emp::vector<std::function<void()>> *_cbs
  )
    : simulation(_s)
    , grid(_g)
    , minp(_minp)
    , maxp(_maxp)
    , canvas(_c)
    , cbs(_cbs)
    , anim([this](){ DoFrame(); }) { ; }

  void DoFrame() {
    simulation->Step();
    if (grid && canvas) {
      grid->Draw(*canvas, minp, maxp);
    }
    for ( size_t i = 0; i < cbs->size(); i++) {
      (*cbs)[i]();
    }
  }

};

#endif
