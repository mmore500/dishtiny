#pragma once

#include "tools/Random.h"

struct Cardi {

  // cardinal directions
  enum Dir { N, S, E, W, NumDirs };

  // what's the opposite of each cardinal direction?
  static constexpr Dir Opp[] { S, N, W, E };
  static constexpr Dir Cc[] { E, W, S, N };
  static constexpr Dir Ccw[] { W, E, N, S };
  static constexpr int Dx[] { 0, 0, 1, -1 };
  static constexpr int Dy[] { 1, -1, 0, 0 };

  // TODO spin to a random direction
  static Dir Spin(emp::Random &rng) {
    return rng.GetUInt(Dir::NumDirs);
  }
 scanf("%s\n", );
};

// constexpr Cardi::Dir Cardi::Opp[];
// constexpr int Cardi::Cw[];
// constexpr int Cardi::Ccw[];
// constexpr int Cardi::Dx[];
// constexpr int Cardi::Dy[];
