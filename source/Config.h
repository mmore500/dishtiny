#pragma once

#define STRINGIFY(s) IFY(s)
#define IFY(s) #s

#include <string>
#include <deque>

#include "base/vector.h"
#include "base/Ptr.h"
#include "config/command_line.h"
#include "config/ArgManager.h"
#include "hardware/EventDrivenGP.h"

#include "ConfigBase.h"
#include "ConfigLevel.h"

class FrameHardware;

class Config : public ConfigBase {

private:

  emp::vector<emp::Ptr<ConfigLevel>> clevs;

  void init() {
    for (size_t l = 0; l < NLEV(); ++l) {

      clevs.push_back(emp::NewPtr<ConfigLevel>());

      if (l == 1) {
        clevs[l]->Set("EVENT_RADIUS", "12");
        clevs[l]->Set("SIGNAL_RADIUS", "36");
      }
      clevs[l]->Read(CONFIGLEVEL_BASENAME() + std::to_string(l) + CONFIGLEVEL_EXTENSION());

    }

    // check conditions on settings
    emp_assert(SEED() > 0);

  }

public:

  static constexpr size_t TAG_WIDTH = 16;

  using TRAIT_TYPE = emp::Ptr<FrameHardware>;
  using chanid_t = uint64_t;
  using hardware_t = emp::EventDrivenGP_AW<TAG_WIDTH, TRAIT_TYPE>;
  using program_t = hardware_t::program_t;
  using inst_lib_t = emp::InstLib<hardware_t>;
  using event_lib_t = emp::EventLib<hardware_t>;
  using event_t = hardware_t::event_t;
  using inbox_t = std::deque<event_t>;

  Config() {
    init();
  }

  Config(std::string config_fname, emp::cl::ArgManager args) {

    Read(config_fname);

    if (args.ProcessConfigOptions(*this, std::cout, config_fname,
          "../ConfigBase.h") == false)
      exit(0);
    if (args.TestUnknown() == false)
      exit(0);

    init();

  }

  ~Config() {
    for (auto &ptr : clevs) ptr.Delete();
  }

  const ConfigLevel& Lev(const size_t lev) const {
    return *clevs[lev];
  }

  void WriteMe(std::ostream & out) {
    Write(out);

    for (size_t l = 0; l < clevs.size(); ++l) {
      std::cout << std::endl << "ConfigLevel" << l << std::endl;
      clevs[l]->Write(out);
    }

  }
};
