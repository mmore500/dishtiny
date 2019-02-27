#pragma once

#include <tuple>
#include <utility>

#include "base/vector.h"
#include "Evolve/World.h"
#include "tools/Random.h"
#include "tools/hash_utils.h"
#include "tools/BitSet.h"

#include "Cardi.h"
#include "FrameCell.h"
#include "Config.h"
#include "Genome.h"
#include "LibraryEvent.h"
#include "LibraryInstruction.h"
#include "Manager.h"
#include "Mutator.h"

class DishWorld : public emp::World<Genome> {

private:

  friend class WebInterface;
  friend class DataHelper;

  const Config &cfg;

  emp::vector<emp::Ptr<emp::Random>> global_rngs;
  emp::vector<emp::Ptr<emp::Random>> local_rngs;
  emp::vector<emp::Ptr<FrameCell>> frames;

  Mutator mut;
  emp::Ptr<Manager> man;

  const Config::inst_lib_t &inst_lib;
  const Config::event_lib_t &event_lib;

public:
  DishWorld(const Config &cfg_, size_t uid_offset=0);

  ~DishWorld();

  void InitSystematics();

  void Pre();

  void Mid();

  void Post();

};
