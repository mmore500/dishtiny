#pragma once
#ifndef DISH_CARDI_HPP_INCLUDE
#define DISH_CARDI_HPP_INCLUDE

#include "../../third-party/Empirical/source/tools/Random.h"

namespace dish {

struct Cardi {

  // cardinal directions
  enum Dir { N, S, E, W, NumDirs };

  // what's the opposite of each cardinal direction?
  static constexpr Dir Opp[] { S, N, W, E };
  static constexpr Dir Cw[] { E, W, S, N };
  static constexpr Dir Ccw[] { W, E, N, S };
  static constexpr int Dx[] { 0, 0, 1, -1 };
  static constexpr int Dy[] { 1, -1, 0, 0 };

};

constexpr Cardi::Dir Cardi::Opp[];
constexpr Cardi::Dir Cardi::Cw[];
constexpr Cardi::Dir Cardi::Ccw[];
constexpr int Cardi::Dx[];
constexpr int Cardi::Dy[];

} // dish

#endif // #ifndef DISH_CARDI_HPP_INCLUDE
