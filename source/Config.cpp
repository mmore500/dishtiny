#pragma once

#include <string>
#include <deque>

#include "base/vector.h"
#include "base/Ptr.h"
#include "config/command_line.h"
#include "config/ArgManager.h"
#include "hardware/EventDrivenGP.h"

#include "Config.h"
#include "ConfigBase.h"
#include "ConfigLevel.h"

void Config::LoadFromFile() {

  Read("base.cfg");

  while (clevs.size() > NLEV()) {
    delete clevs.back();
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
  emp_assert(TREATMENT_DESCRIPTOR().rfind("treat=", 0) == 0);
}

Config::Config() {
  for (size_t l = 0; l < NLEV(); ++l) {
    clevs.push_back(emp::NewPtr<ConfigLevel>());
    if (l == 1) {
      clevs[l]->Set("EVENT_RADIUS", "12");
      clevs[l]->Set("SIGNAL_RADIUS", "36");
    }
  }
  // check conditions on settings
  emp_assert(SEED() > 0);
  emp_assert(TREATMENT_DESCRIPTOR().rfind("treat=", 0) == 0);
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

bool Config::TimingFun(const size_t update) const {
  return update % SNAPSHOT_FREQUENCY() < SNAPSHOT_LENGTH();
}
