#pragma once

#include <iostream>

#include "web/Canvas.h"
#include "web/Animate.h"

#include "Grid.h"

template <typename STATE_TYPE>
class GridAnimator : public emp::web::Animate {
public:

  GridStack<STATE_TYPE>& stack;
  size_t lev;
  const STATE_TYPE *minp;
  const STATE_TYPE *maxp;
  emp::web::Canvas& canvas;
  emp::vector<std::function<void()>>& cbs_beforedraw;
  emp::vector<std::function<void()>>& cbs_afterdraw;

public:
  GridAnimator(
    GridStack<STATE_TYPE>& _stack,
    size_t _lev,
    STATE_TYPE *_minp,
    STATE_TYPE *_maxp,
    emp::web::Canvas& _canvas,
    emp::vector<std::function<void()>>& _cbs_beforedraw,
    emp::vector<std::function<void()>>& _cbs_afterdraw)
  : emp::web::Animate([this](){
    for (size_t i = 0; i < cbs_beforedraw.size(); ++i) {
      cbs_beforedraw[i]();
    }
    Draw(lev);
    for (size_t i = 0; i < cbs_afterdraw.size(); ++i) {
      cbs_afterdraw[i]();
    }
  })
  , stack(_stack)
  , lev(_lev)
  , minp(_minp)
  , maxp(_maxp)
  , canvas(_canvas)
  , cbs_beforedraw(_cbs_beforedraw)
  , cbs_afterdraw(_cbs_afterdraw) { ; }

  /*
   * Draw a Grid from stored GridStack onto a canvas.
   * @param lev The stack index to the Grid to be drawn.
   * @param line_color The background line color for the grid.
   */
  void const Draw(
    size_t lev,
    std::string line_color="black"
  )
  {
    // Determine the canvas info.
    const size_t canvas_w = canvas.GetWidth();
    const size_t canvas_h = canvas.GetHeight();

    // Determine the cell width & height.
    const size_t cell_w = canvas_w / stack.GetWidth();
    const size_t cell_h = canvas_h / stack.GetHeight();

    // Determine the realized grid width and height on the canvas.
    const size_t grid_w = cell_w * stack.GetWidth();
    const size_t grid_h = cell_h * stack.GetHeight();

    // Center the grid on the canvas if there's extra room.
    const size_t offset_x = (canvas_w <= grid_w) ? 0 : (canvas_w - grid_w) / 2;
    const size_t offset_y = (canvas_h <= grid_h) ? 0 : (canvas_h - grid_h) / 2;

    canvas.Clear();

    // Determine minimum, maximum value
    STATE_TYPE minv, maxv;
    if (minp && maxp) {
      minv = *minp;
      maxv = *maxp;
    } else {
      auto minmax = std::minmax_element(stack.Begin(lev), stack.End(lev));
      minv = minp ? *minp : *(minmax.first);
      maxv = maxp ? *maxp : *(minmax.second);
    }

    std::cout << minv << std::endl;
    std::cout << maxv << std::endl << std::endl;


    // Calculate scaling factor
    STATE_TYPE diff = maxv - minv;
    if (diff == 0) diff += 1;

    // Setup a black background for the grid.
    canvas.Rect(0, 0, canvas.GetWidth(), canvas.GetHeight(), line_color);

    // Fill out the grid!
    size_t id = 0;
    for (size_t row = 0; row < stack.GetHeight(); row++) {
      const size_t cur_y = offset_y + row*cell_h;
      for (size_t col = 0; col < stack.GetWidth(); col++) {
        const size_t cur_x = offset_x + col*cell_w;
        const STATE_TYPE state = stack.Get(lev, id++);

        // normalize and contain graphic intensity
        int val = ((state - minv) * 255) / diff;
        int v1,v2,v3;
        if (state < minv) {
          v1 = 255;
          v2 = 0;
          v3 = 0;
        } else if (state > maxv) {
          v1 = 0;
          v2 = 0;
          v3 = 255;
        } else {
          v1 = val;
          v2 = val;
          v3 = val;
        }
        const std::string & cur_color = emp::ColorRGB(v1, v2, v3);
        canvas.Rect(cur_x, cur_y, cell_w, cell_h, cur_color, line_color);
      }
    }
  }


};
