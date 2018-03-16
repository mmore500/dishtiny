#pragma once

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
      Draw(grid, *canvas, minp, maxp);
    }
    for ( size_t i = 0; i < cbs->size(); i++) {
      (*cbs)[i]();
    }
  }

  /// Draw a state grid onto a canvas.
  /// @param canvas The Canvas to draw on.
  /// @param minp The value to associate with minimum graphic intensity.
  /// @param maxp The value to associate with maximum graphic intensity. Pass a
  /// @param color_map Mapping of values to the colors with which they should be associated.
  /// @param line_color The background line color for the grid.
  void const Draw(
    Grid & grid,
    emp::web::Canvas canvas,
    const STATE_TYPE *minp,
    const STATE_TYPE *maxp,
    std::string line_color="black"
  )
  {
    // Determine the canvas info.
    const size_t canvas_w = canvas.GetWidth();
    const size_t canvas_h = canvas.GetHeight();

    // Determine the cell width & height.
    const size_t cell_w = canvas_w / this->GetWidth();
    const size_t cell_h = canvas_h / this->GetHeight();

    // Determine the realized grid width and height on the canvas.
    const size_t grid_w = cell_w * this->GetWidth();
    const size_t grid_h = cell_h * this->GetHeight();

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
      auto minmax = std::minmax_element(std::begin(grid), std::end(grid));
      minv = minp ? *minp : *(minmax.first);
      maxv = maxp ? *maxp : *(minmax.second);
    }

    // Calculate scaling factor
    STATE_TYPE diff = maxv - minv;
    if (diff == 0) diff += 1;

    // Setup a black background for the grid.
    canvas.Rect(0, 0, canvas.GetWidth(), canvas.GetHeight(), line_color);

    // Fill out the grid!
    size_t id = 0;
    for (size_t row = 0; row < grid.GetHeight(); row++) {
      const size_t cur_y = offset_y + row*cell_h;
      for (size_t col = 0; col < grid.GetWidth(); col++) {
        const size_t cur_x = offset_x + col*cell_w;
        const STATE_TYPE state = grid[id++];

        // normalize and contain graphic intensity
        int val = ((state - minv) * 255) / diff;
        int v1,v2,v3;
        if (state < minv || state > maxv) {
          v1 = 255;
          v2 = 0;
          v3 = 0;
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
