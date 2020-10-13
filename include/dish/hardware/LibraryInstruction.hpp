#pragma once
#ifndef DISH_HARDWARE_LIBRARYINSTRUCTION_HPP_INCLUDE
#define DISH_HARDWARE_LIBRARYINSTRUCTION_HPP_INCLUDE

#include <cmath>
#include <functional>
#include <limits>
#include <memory>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/hardware/InstLib.h"
#include "../../../third-party/Empirical/source/tools/Random.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../config/Config.hpp"
#include "../topology/GeometryHelper.hpp"
#include "../trait/FrameHardware.hpp"
#include "../world/DishWorld.hpp"

namespace dish {

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

} // namespace dish

#endif // #ifndef DISH_HARDWARE_LIBRARYINSTRUCTION_HPP_INCLUDE
