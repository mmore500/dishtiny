#pragma once
#ifndef DISHTINY_LIBRARYINSTRUCTION_HPP_INCLUDE
#define DISHTINY_LIBRARYINSTRUCTION_HPP_INCLUDE

#include <functional>
#include <cmath>
#include <limits>
#include <memory>

#include "../../third-party/Empirical/source/base/assert.h"
#include "../../third-party/Empirical/source/tools/Random.h"
#include "../../third-party/Empirical/source/tools/string_utils.h"
#include "../../third-party/Empirical/source/hardware/InstLib.h"

#include "FrameHardware.hpp"
#include "Config.hpp"
#include "GeometryHelper.hpp"
#include "DishWorld.hpp"

class LibraryInstruction {

public:

  using hardware_t = Config::hardware_t;
  using inst_lib_t = Config::inst_lib_t;
  using inst_t = inst_lib_t::inst_t;
  using state_t = hardware_t::State;

private:

  static void TRL(
    hardware_t & hw,
    const double dir_arg,
    const size_t lev,
    const bool inherit_regulators,
    const Config &cfg
  );

public:

  static void InitDefault(inst_lib_t &il);

  static void InitDefaultDup(inst_lib_t &il);

  static void InitInternalActions(inst_lib_t &il, const Config &cfg);

  static void InitExternalActions(inst_lib_t &il, const Config &cfg);

  static void InitExternalSensors(inst_lib_t &il, const Config &cfg);

  static void InitInternalSensors(inst_lib_t &il, const Config &cfg);

  static void InitSpikerActions(inst_lib_t &il, const Config &cfg);

  static void InitDevoActions(inst_lib_t &il, const Config &cfg);

  static const inst_lib_t& Make(const Config &cfg);

};

#endif // #ifndef DISHTINY_LIBRARYINSTRUCTION_HPP_INCLUDE
