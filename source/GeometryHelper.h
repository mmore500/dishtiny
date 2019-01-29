#pragma once

#include "base/assert.h"
#include "base/vector.h"
#include "tools/math.h"

#include "Cardi.h"
#include "Config.h"

class GeometryHelper {

private:

  Config &cfg;

public:

  GeometryHelper(Config &cfg_): cfg(cfg_) { ; }

  /*
   * Local geometry helper function.
   */
  inline size_t GetLocalX(size_t pos) const { return pos % cfg.GRID_W(); }

  /*
   * Local geometry helper function.
   */
  inline size_t GetLocalY(size_t pos) const { return pos / cfg.GRID_W(); }


  /*
   * Local geometry helper function.
   */
  inline size_t GetLocalSize() const { return cfg.GRID_H() * cfg.GRID_W(); }

  /*
   * Local eometry helper function.
   */
  inline size_t GetLocalPos(int x, int y) const {
    return emp::Mod(y, cfg.GRID_H()) * cfg.GRID_W() + emp::Mod(x, cfg.GRID_W());
  }

  emp::vector<size_t> CalcLocalNeighs(size_t pos) {

    emp::vector<size_t> res;

    for(size_t d = 0; d < Cardi::NumDirs; ++d) {
      res.push_back(GetLocalPos(
          ((int)GetLocalX(pos)) + Cardi::Dx[d],
          ((int)GetLocalY(pos)) + Cardi::Dy[d]
        ));
    }

    return res;

  }

  /*
   * Global geometry helper function.
   */
  inline size_t GetGlobalX(size_t pos) const {
    /* TODO change this for MPI implementation */
    return GetLocalX(pos);
  }

  /*
   * Global geometry helper function.
   */
  inline size_t GetGlobalY(size_t pos) const {
    /* TODO change this for MPI implementation */
    return GetLocalY(pos);
  }

  /*
   * Global eometry helper function.
   */
  inline size_t GetGlobalPos(int x, int y) const {
    /* TODO change this for MPI implementation */
    return GetLocalPos(x,y);
  }

};
