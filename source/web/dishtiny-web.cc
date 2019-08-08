#include <string>
#include <unistd.h>

#include "config/command_line.h"
#include "config/ArgManager.h"
#include "web/UrlParams.h"

#include "../Config.h"
#include "../DishWorld.h"
#include "../WebInterface.h"

#include "../Config.cpp"
#include "../DishWorld.cpp"
#include "../FrameHardware.cpp"
#include "../FrameCell.cpp"
#include "../Manager.cpp"
#include "../LibraryInstruction.cpp"

struct ConfigSetup {

  ConfigSetup(Config & cfg) {
    auto specs = emp::ArgManager::make_builtin_specs(&cfg);
    specs["treat"] = emp::ArgSpec(
      1,
      "treatment",
      {},
      [](std::optional<emp::vector<std::string>> res){
        static bool any_res{false};
        if (res) {
          any_res = true;
          const std::string arg = "/request/treat=" + (*res)[0];
          chdir(arg.c_str());
        } else if (!any_res) {
          chdir("/request");
        }
      }
    );
    emp::ArgManager am(emp::web::GetUrlParams(), specs);
    am.CallbackArg("treat");
    cfg.LoadFromFile();
    am.UseCallbacks();
    if (am.HasUnused()) std::exit(EXIT_FAILURE);
  }

};

Config cfg;
ConfigSetup cs(cfg);
WebInterface i(cfg);

int main()
{

  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  cfg.WriteMe(std::cout);
  std::cout << "==============================\n" << std::endl;

  i.InitializeViewers(0);
  i.Redraw(0);

  return 0;
}
