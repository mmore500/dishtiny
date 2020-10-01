#pragma once
#ifndef DISHTINY_DISHWORLD_HPP_INCLUDE
#define DISHTINY_DISHWORLD_HPP_INCLUDE

#include <tuple>
#include <utility>

#include "../../third-party/Empirical/source/base/vector.h"
#include "../../third-party/Empirical/source/Evolve/World.h"
#include "../../third-party/Empirical/source/tools/Random.h"
#include "../../third-party/Empirical/source/tools/hash_utils.h"
#include "../../third-party/Empirical/source/tools/BitSet.h"

#include "Cardi.hpp"
#include "FrameCell.hpp"
#include "Config.hpp"
#include "Genome.hpp"
#include "LibraryEvent.hpp"
#include "LibraryInstruction.hpp"
#include "Manager.hpp"
#include "Mutator.hpp"

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
  const Config::inst_lib_t &inst_lib_spiker;
  const Config::event_lib_t &event_lib;

  void Step();

public:
  DishWorld(const Config &cfg_, size_t uid_offset=0);

  ~DishWorld();

  void InitSystematics();

  void LoadPopulation();

  void GeneratePopulation();

  const Manager& GetManager() const;

  FrameCell & GetFrame(const size_t loc);

};

#endif // #ifndef DISHTINY_DISHWORLD_HPP_INCLUDE
