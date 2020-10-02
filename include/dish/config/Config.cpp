#pragma once
#ifndef DISHTINY_CONFIG_CPP_INCLUDE
#define DISHTINY_CONFIG_CPP_INCLUDE

#include <string>
#include <deque>

#include "../../../third-party/Empirical/source/base/Ptr.h"
#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/Empirical/source/config/command_line.h"
#include "../../../third-party/Empirical/source/config/ArgManager.h"
#include "../../../third-party/Empirical/source/hardware/EventDrivenGP.h"

#include "Config.hpp"
#include "ConfigBase.hpp"
#include "ConfigLevel.hpp"

namespace dish {

void Config::LoadFromFile() {

  Read("base.cfg");

  while (clevs.size() > NLEV()) {
    clevs.back().Delete();
    clevs.pop_back();
  }

  for (size_t l = 0; l < NLEV(); ++l) {

    if (l == clevs.size()) clevs.push_back(emp::NewPtr<ConfigLevel>());

    clevs[l]->Read(
      CONFIGLEVEL_BASENAME() + std::to_string(l) + CONFIGLEVEL_EXTENSION()
    );

  }
  // check conditions on settings
  emp_assert(SEED() > 0);
}

Config::Config() {
  for (size_t l = 0; l < NLEV(); ++l) {
    clevs.push_back(emp::NewPtr<ConfigLevel>());
    if (l == 1) {
      clevs[l]->Set("EVENT_RADIUS", "6");
      clevs[l]->Set("SIGNAL_RADIUS", "24");
    }
  }
  // check conditions on settings
  emp_assert(SEED() > 0);
}

Config::~Config() {
  for (auto &ptr : clevs) ptr.Delete();
}

const ConfigLevel& Config::Lev(const size_t lev) const {
  return *clevs[lev];
}

void Config::WriteMe(std::ostream & out) const {
  Write(out);

  for (size_t l = 0; l < clevs.size(); ++l) {
    out << std::endl << "ConfigLevel=" << l << std::endl;
    clevs[l]->Write(out);
  }

}

} // namespace dish

#endif // #ifndef DISHTINY_CONFIG_CPP_INCLUDE
