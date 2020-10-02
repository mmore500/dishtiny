#pragma once
#ifndef DISH_GEOMETRYHELPER_HPP_INCLUDE
#define DISH_GEOMETRYHELPER_HPP_INCLUDE

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/Empirical/source/tools/math.h"

#include "../topology/Cardi.hpp"
#include "../config/Config.hpp"

namespace dish {

class GeometryHelper {

private:

  const Config &cfg;

public:

  GeometryHelper(const Config &cfg_): cfg(cfg_) { ; }

  /*
   * Local geometry helper function.
   */
  inline size_t GetLocalX(const size_t pos) const { return pos % cfg.GRID_W(); }

  /*
   * Local geometry helper function.
   */
  inline size_t GetLocalY(const size_t pos) const { return pos / cfg.GRID_W(); }

  /*
   * Return subgrid size.
   */
  inline size_t SubGridSize() const { return cfg.SUBGRIDS(); }

  /*
   * Local geometry helper function.
   */
  inline size_t GetLocalSize() const { return cfg.GRID_H() * cfg.GRID_W(); }

  /*
   * Local geometry helper function.
   */
  inline size_t GetLocalPos(const int x, const int y) const {
    return (
      emp::Mod(y, static_cast<int>(cfg.GRID_H())) * cfg.GRID_W()
      + emp::Mod(x, static_cast<int>(cfg.GRID_W()))
    );
  }

  emp::vector<size_t> CalcLocalNeighs(const size_t pos) const {

    emp::vector<size_t> res;

    for(size_t d = 0; d < Cardi::NumDirs; ++d) {
      res.push_back( CalcLocalNeigh(pos, d) );
    }

    return res;

  }

  inline size_t CalcLocalNeigh(const size_t pos, const size_t dir) const {

    return GetLocalPos(
      static_cast<int>(GetLocalX(pos)) + Cardi::Dx[dir],
      static_cast<int>(GetLocalY(pos)) + Cardi::Dy[dir]
    );

  }

  /*
   * Global geometry helper function.
   */
  inline size_t GetGlobalX(const size_t pos) const {
    /* TODO change this for MPI implementation */
    return GetLocalX(pos);
  }

  /*
   * Global geometry helper function.
   */
  inline size_t GetGlobalY(const size_t pos) const {
    /* TODO change this for MPI implementation */
    return GetLocalY(pos);
  }

  /*
   * Global eometry helper function.
   */
  inline size_t GetGlobalPos(const int x, const int y) const {
    /* TODO change this for MPI implementation */
    return GetLocalPos(x,y);
  }

};

} // namespace dish

#endif // #ifndef DISH_GEOMETRYHELPER_HPP_INCLUDE
