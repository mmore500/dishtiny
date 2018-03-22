#pragma once

#include <algorithm>

#include "base/vector.h"

#include "Grid.h"

template <typename STATE_TYPE>
class GridStack {
private:
  // 2D grid geometry helper
  GridSpec spec;
  // store a 3D "stack" of grids
  emp::vector<Grid<STATE_TYPE>> grids;

  const size_t depth;

public:
  /*
   * _e describes the value an empty cell.
   * _spec describes the 2D geometry of the stacked grids.
   * _depth debes how deep to go.
   */
  GridStack(STATE_TYPE _e, GridSpec& _spec, size_t _depth)
  : spec(_spec)
  , depth(_depth)
  {
    for (size_t i = 0; i < depth; ++i) {
      grids.push_back(*emp::NewPtr<Grid<STATE_TYPE>>(_e, _spec));
    }
  }

  /*
   * Accessor function.
   */
  inline size_t GetWidth() const { return spec.width; }
  /*
   * Accessor function.
   */
  inline size_t GetHeight() const { return spec.height; }

  /*
   * Accessor function for const_iterator to a Grid.
   */
  inline typename
  emp::vector<STATE_TYPE>::const_iterator Begin(size_t lev) const {
    return grids[lev].Begin();
  }
  /*
   * Accessor function for const_iterator to a Grid.
   */
  inline typename
  emp::vector<STATE_TYPE>::const_iterator End(size_t lev) const {
    return grids[lev].End();
  }

  /*
   * Accessor function.
   */
  inline size_t GetArea() const { return spec.area; }
  /*
   * Accessor function.
   */
  inline size_t GetDepth() const { return depth; }
  /*
   * Accessor function.
   */
  inline size_t GetVolume() const { return GetArea() * GetDepth(); }

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
   * Reset all values in the GridStack to STATE_TYPE _e.
   */
  void inline Reset() {
    for (auto it = grids.begin(); it != grids.end(); ++it) {
      it->Reset();
    }
  }

  /*
   * Accessor function (provides write access).
   */
  inline STATE_TYPE& operator()(size_t lev, int x, int y) {
    return grids[lev](spec.GetID(x,y));
  }

  /*
   * Accessor function (provides write access).
   */
  inline STATE_TYPE& operator()(size_t lev, size_t id) {
    return grids[lev](id);
  }

  /*
   * Accessor function.
   */
  inline STATE_TYPE Get(size_t lev, int x, int y) const {
    return grids[lev].Get(spec.GetID(x,y));
  }

  /*
   * Accessor function.
   */
  inline STATE_TYPE Get(size_t lev, size_t id) const {
    return grids[lev].Get(id);
  }

private:
  /*
   * Accessor function used by friend class GridAnimator.
   */
  Grid<STATE_TYPE>& GetGrid(size_t lev) {
    return grids[lev];
  }

};
