#pragma once

#include <memory>

#include "base/vector.h"
#include "tools/math.h"
#include "tools/FunctionSet.h"

#include "Cardi.h"

class TileGrid {

private:
  const size_t height;
  const size_t width;
  emp::vector<ActivationHandle<Tile>> tiles;

  emp::Random global_rng;

public:
  TileGrid(
    DishtinyConfig &config,
    std::function<size_t(size_t i)>  make_uid
  ) : height(config.GRID_H())
  , width(config.GRID_W())
  , global_rng(config.SEED())
  {
    for(size_t i = 0; i <  config.GRID_H() * config.GRID_W(); ++i) {
      tiles.emplace_back(make_uid(i), config);
    }

    for(size_t i = 0; i < config.GRID_H() * config.GRID_W(); ++i) {
      emp::vec<ActivationHandle<Tile>&> neighs;
      for(size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
        neighs.push_back(tiles[GetID(
            ((int)GetX(i)) + Cardi::Dx[d],
            ((int)GetY(i)) + Cardi::Dy[d]
          )]);
      }
      tiles.[i].SetNeigh(neighs);
    }

  }

  // /*
  //  * Accessor function.
  //  */
  // inline size_t GetWidth() const { return width; }
  // /*
  //  * Accessor function.
  //  */
  // inline size_t GetHeight() const { return height; }
  // /*
  //  * Accessor function.
  //  */
  // inline ActivationHandle<Tile>& Get(int x, int y) const { return *this[spec.GetID(x,y)]; }

};
