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

void Config::init() {
  for (size_t l = 0; l < NLEV(); ++l) {

    clevs.push_back(emp::NewPtr<ConfigLevel>());

    if (l == 1) {
      clevs[l]->Set("EVENT_RADIUS", "12");
      clevs[l]->Set("SIGNAL_RADIUS", "36");
    }
    #ifndef EMSCRIPTEN
    clevs[l]->Read(CONFIGLEVEL_BASENAME() + std::to_string(l) + CONFIGLEVEL_EXTENSION());
    #endif
  }

  // check conditions on settings
  emp_assert(SEED() > 0);
  emp_assert(TREATMENT_DESCRIPTOR().rfind("treat=", 0) == 0);
}

Config::Config() {
  init();
}

Config::Config(std::string config_fname, emp::cl::ArgManager args) {

  Read(config_fname);

  if (args.ProcessConfigOptions(*this, std::cout, config_fname,
        "../ConfigBase.h") == false)
    exit(0);
  if (args.TestUnknown() == false)
    exit(0);

  init();

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
