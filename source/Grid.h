#pragma once

#include <algorithm>

#include "base/vector.h"

#include "GridSpec.h"

template <typename STATE_TYPE>
class Grid {

private:
  GridSpec spec;
  STATE_TYPE empty;
  emp::vector<STATE_TYPE> states;

public:
  Grid(STATE_TYPE _e, GridSpec & _spec)
  : spec(_spec)
  , empty(_e)
  , states(spec.GetArea()) { ; }

  inline size_t GetWidth() const { return spec.GetWidth(); }
  inline size_t GetHeight() const { return spec.GetHeight(); }
  inline size_t GetArea() const { return spec.GetArea(); }

  inline size_t GetX(size_t id) const { return spec.GetX(id); }
  inline size_t GetY(size_t id) const { return spec.GetY(id); }
  inline size_t GetID(int x, int y) const { return spec.GetID(x,y); }

  void inline Reset() {
    std::fill(states.begin(), states.end(), empty);
  }

  inline STATE_TYPE& operator()(int x, int y) {
    return states[spec.GetID(x,y)];
  }
  inline STATE_TYPE& operator()(size_t id) { return states[id]; }

  inline STATE_TYPE Get(int x, int y) const { return states[spec.GetID(x,y)]; }
  inline STATE_TYPE Get(size_t id) const { return states[id]; }

};
