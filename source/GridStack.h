#pragma once

#include <algorithm>

#include "base/vector.h"

#include "Grid.h"

template <typename STATE_TYPE>
class GridStack {
private:
  GridSpec spec;
  emp::vector<Grid<STATE_TYPE>> grids;
  const size_t depth;

public:
  GridStack(STATE_TYPE _e, GridSpec & _spec, size_t _depth)
  : spec(_spec)
  , depth(_depth)
  {
    for (size_t i = 0; i < _depth; ++i) {
        grids.push_back(*(new Grid<STATE_TYPE>(_e, _spec)));
    }
  }

  inline size_t GetWidth() const { return spec.width; }
  inline size_t GetHeight() const { return spec.height; }

  inline size_t GetArea() const { return spec.area; }
  inline size_t GetDepth() const { return depth; }
  inline size_t GetVolume() const { return GetArea() * GetDepth(); }

  inline size_t GetX(size_t id) const { return spec.GetX(id); }
  inline size_t GetY(size_t id) const { return spec.GetY(id); }
  inline size_t GetID(int x, int y) const { return spec.GetID(x,y); }

  void inline Reset() {
    for (auto it = grids.begin(); it != grids.end(); ++it) {
      it->Reset();
    }
  }

  inline STATE_TYPE& operator()(size_t lev, int x, int y) {
    return grids[lev](spec.GetID(x,y));
  }

  inline STATE_TYPE& operator()(size_t lev, size_t id) {
    return grids[lev](id);
  }

  inline STATE_TYPE Get(size_t lev, int x, int y) const {
    return grids[lev].Get(spec.GetID(x,y));
  }

  inline STATE_TYPE Get(size_t lev, size_t id) const {
    return grids[lev].Get(id);
  }


};
