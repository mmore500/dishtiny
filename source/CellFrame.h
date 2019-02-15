#pragma once

#include <algorithm>
#include <list>

#include "tools/Random.h"
#include "tools/random_utils.h"

#include "Cardi.h"
#include "Config.h"
#include "GeometryHelper.h"

//forward declaration
class Manager;

class CellFrame {

private:

  size_t facing;
  Manager &man;
  emp::Random &local_rng;

  emp::vector<size_t> neighs;
  const size_t pos;

  // arranged by incoming direction
  emp::vector<bool> inbox_active;

  size_t msgdir;

public:

  CellFrame(
    emp::Ptr<emp::Random> local_rng_,
    Config &cfg,
    Manager &man_,
    size_t pos_
  ) : facing(Cardi::Spin(*local_rng_))
    , man(man_)
    , local_rng(*local_rng_)
    , neighs(GeometryHelper(cfg).CalcLocalNeighs(pos_))
    , pos(pos_)
    , inbox_active()
  {
    Reset();
  }

  void Reset() {
    facing = Cardi::Spin(local_rng);
    emp::vector<bool>(Cardi::Dir::NumDirs).swap(inbox_active);
  }

  const size_t GetPos() {
    return pos;
  }

  size_t GetFacing() {
    return facing;
  }

  void SetFacing(size_t new_facing) {
    facing = new_facing;
  }

  void SpinFacing() {
    SetFacing(Cardi::Spin(local_rng));
  }

  size_t GetMsgDir() {
    return msgdir;
  }

  void SetMsgDir(size_t new_dir) {
    msgdir = new_dir;
  }

  Manager& GetManager() {
    return man;
  }

  const size_t GetNeigh(size_t dir) {
    return neighs[dir];
  }

  void SetInboxActivity(size_t dir, bool state) {
    inbox_active[dir] = state;
  }

  bool CheckInboxActivity(size_t dir) {
    return inbox_active[dir];
  }

};
