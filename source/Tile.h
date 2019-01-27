#pragma once

#include <optional>


#include "base/vector.h"
#include "tools/ActivationHandle.h"
#include "tools/hash_utils.h"
#include "tools/Random.h"

#include "Cardi.h"
#include "CellFrame.h"
#include "CellHardware.h"

class Tile {

private:

  emp::Random &rng;

  CellFrame &frame;
  CellFrameReq &frame_next;

  CellHardware &cpu;

  emp::vec<ActivationHandle<Tile>&> neighs;

  // optional depending on whether the neighbor is alive
  std::function GetNeigh<>;

public:

  Tile(
    size_t uid_,
    DishtinyConfig &config
  ) :
  GetNeigh(GetNeigh_)
  , rng(emp::szudzik_hash(config.SEED(),uid_))
  , frame(TODO)
  , hardware(TODO)
  { ; }

  void SetNeigh(emp::vec<ActivationHandle<Tile>&> neigh_in) {
    for (auto x : neigh_in) {
      neigh.push_back(x);
    }
  }

  CellFrame& GetFrame() {
    return frame;
  }



}
