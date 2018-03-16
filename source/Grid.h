#pragma once

#include <algorithm>

#include "base/vector.h"

#include "GridSpec.h"

template <typename STATE_TYPE>
class Grid {

private:
  // 2D geomeotry helper
  GridSpec spec;
  // what do empty cells look like?
  STATE_TYPE empty;
  
  emp::vector<STATE_TYPE> states;

public:
  Grid(STATE_TYPE _e, GridSpec & _spec)
  : spec(_spec)
  , empty(_e)
  , states(spec.GetArea()) { ; }

  /*
   * Accessor function.
   */
  inline size_t GetWidth() const { return spec.GetWidth(); }
  /*
   * Accessor function.
   */
  inline size_t GetHeight() const { return spec.GetHeight(); }
  /*
   * Accessor function.
   */
  inline size_t GetArea() const { return spec.GetArea(); }

  /*
   * Geometry helper function.
   */
  inline size_t GetX(size_t id) const { return spec.GetX(id); }
  /*
   * Geometry helper function.
   */
  inline size_t GetY(size_t id) const { return spec.GetY(id); }
  /*
   * Geometry helper function.
   */
  inline size_t GetID(int x, int y) const { return spec.GetID(x,y); }

  /*
   * Reset all values in the Grid to STATE_TYPE _e.
   */
  void inline Reset() {
    std::fill(states.begin(), states.end(), empty);
  }

  /*
   * Accessor function (provides write access).
   */
  inline STATE_TYPE& operator()(int x, int y) {
    return states[spec.GetID(x,y)];
  }
  /*
   * Accessor function (provides write access).
   */
  inline STATE_TYPE& operator()(size_t id) { return states[id]; }

  /*
   * Accessor function.
   */
  inline STATE_TYPE Get(int x, int y) const { return states[spec.GetID(x,y)]; }
  /*
   * Accessor function.
   */
  inline STATE_TYPE Get(size_t id) const { return states[id]; }

};
