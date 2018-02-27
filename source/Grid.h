#ifndef GRIDF_H
#define GRIDF_H

#include <iostream>
#include <algorithm>

#include "base/vector.h"
#include "tools/Random.h"
#include "web/Canvas.h"
#include "web/color_map.h"

#include "Simulation.h"
#include "macro.h"

template <typename STATE_TYPE>
class Grid {
private:
  size_t width;
  size_t height;
  STATE_TYPE empty;

public:
  emp::vector<STATE_TYPE> states;

public:
  Grid(size_t _w, size_t _h, STATE_TYPE _e)
  : width(_w)
  , height(_h)
  , empty(_e)
  , states(_w*_h) { ; }
  Grid(const Grid &) = default;
  ~Grid() { ; }
  // Grid & operator=(const Grid &) = default;

  size_t GetWidth() const { return width; }
  size_t GetHeight() const { return height; }
  size_t GetSize() const { return states.size(); }

  void inline reset() {
    std::fill(states.begin(), states.end(), empty);
  }

  // Helper functions
  inline size_t GetX(size_t id) const { return id % width; }
  inline size_t GetY(size_t id) const { return id / width; }
  inline size_t GetID(size_t x, size_t y) const { return mod(y, height) * width + mod(x, width); }

  void Incr(size_t x, size_t y, STATE_TYPE v) {
    states[GetID(x,y)] += v;
  }

  void Incr(size_t id, STATE_TYPE v) {
    states[id] += v;
  }

  void Set(size_t id, STATE_TYPE v) {
    states[id] = v;
  }

  void Set(size_t x, size_t y, STATE_TYPE v) {
    states[GetID(x,y)] = v;
  }

  STATE_TYPE operator()(size_t x, size_t y) { return states[GetID(x,y)]; }
  STATE_TYPE operator()(size_t id) { return states[id]; }
  STATE_TYPE operator[](size_t id) { return states[id]; }

  /// Draw a state grid onto a canvas.
  /// @param canvas The Canvas to draw on.
  /// @param minp The value to associate with minimum graphic intensity.
  /// @param maxp The value to associate with maximum graphic intensity. Pass a
  /// @param color_map Mapping of values to the colors with which they should be associated.
  /// @param line_color The background line color for the grid.
  void const Draw(
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
      auto minmax = std::minmax_element(std::begin(states), std::end(states));
      minv = minp ? *minp : *(minmax.first);
      maxv = maxp ? *maxp : *(minmax.first);
    }

    // Calculate scaling factor
    STATE_TYPE diff = maxv - minv;
    if (diff == 0) diff += 1;

    // Setup a black background for the grid.
    canvas.Rect(0, 0, canvas.GetWidth(), canvas.GetHeight(), line_color);

    // Fill out the grid!
    size_t id = 0;
    for (size_t row = 0; row < this->GetHeight(); row++) {
      const size_t cur_y = offset_y + row*cell_h;
      for (size_t col = 0; col < this->GetWidth(); col++) {
        const size_t cur_x = offset_x + col*cell_w;
        const STATE_TYPE state = states[id++];

        // normalize and contain graphic intensity
        int val = ((state - minv) * 255) / diff;
        val = std::min(std::max(0, val), 255);

        const std::string & cur_color = emp::ColorRGB(val, val, val);
        canvas.Rect(cur_x, cur_y, cell_w, cell_h, cur_color, line_color);
      }
    }
  }

};

#endif
