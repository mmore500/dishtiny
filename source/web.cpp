#include <string>
#include <unistd.h>

#include <emscripten.h>
#include <zlib.h>

#include "cereal/include/cereal/cereal.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "cereal/include/cereal/types/array.hpp"
#include "cereal/include/cereal/types/vector.hpp"
#include "cereal/include/cereal/types/map.hpp"
#include "cereal/include/cereal/types/unordered_map.hpp"
#include "cereal/include/cereal/types/string.hpp"
#include "conduit/include/uitsl/polyfill/filesystem.hpp"
#include "conduit/include/uitsl/utility/inflate.hpp"
#include "conduit/include/uitsl/utility/untar.hpp"
#include "Empirical/source/config/command_line.h"
#include "Empirical/source/config/ArgManager.h"
#include "Empirical/source/tools/keyname_utils.h"
#include "Empirical/source/web/UrlParams.h"

#include "dishtiny/Config.hpp"
#include "dishtiny/DishWorld.hpp"
#include "dishtiny/WebInterface.hpp"

#include "dishtiny/Config.cpp"
#include "dishtiny/DishWorld.cpp"
#include "dishtiny/FrameHardware.cpp"
#include "dishtiny/FrameCell.cpp"
#include "dishtiny/Manager.cpp"
#include "dishtiny/LibraryInstruction.cpp"
#include "dishtiny/LibraryInstructionSpiker.cpp"
#include "dishtiny/ManagerConnection.cpp"


Config cfg;

// for logging
void pwd() {
  std::cout << std::filesystem::absolute(".") << std::endl;
  for (auto& p: std::filesystem::directory_iterator(".")) {
    std::cout << p.path() << std::endl;
  }
}

void run() {

  // apply configuration query params and config files to Config
  auto specs = emp::ArgManager::make_builtin_specs(&cfg);
  emp::ArgManager am(emp::web::GetUrlParams(), specs);
  cfg.LoadFromFile();
  am.UseCallbacks();
  if (am.HasUnused()) std::exit(EXIT_FAILURE);

  // log configuraiton settings
  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  cfg.WriteMe(std::cout);
  std::cout << "==============================\n" << std::endl;

  // initialize persistent web interface
  auto i = new WebInterface(cfg);

  // set up web interface
  i->InitializeViewers(0);
  i->Redraw(0);

  std::cout << "web viewer load SUCCESS" << std::endl;

  // once we're done setting up, turn off the loading modal
  emscripten_run_script("$('.modal').modal('hide');");
}

// unpack treatment directory and jump in there, then call run
void treatget_callback(const char *filename) {

  uitsl::untar( uitsl::inflate( filename ) );

  std::filesystem::remove(filename);

  std::cout << "changing directory..." << std::endl;
  chdir("treatment_directory");
  pwd();

  run();

}

int main()
{

  const std::string treatment_descriptor{
  (
    emp::web::GetUrlParams().count("TREATMENT_DESCRIPTOR") == 1
    && emp::web::GetUrlParams().find("TREATMENT_DESCRIPTOR")->second.size() == 1
    && (
      emp::web::GetUrlParams().find("TREATMENT_DESCRIPTOR")->second.front()
      != "unspecified"
    )
  )
  ? emp::web::GetUrlParams().find("TREATMENT_DESCRIPTOR")->second.front()
  : "unspecified"
  };

  std::cout << "treatment_descriptor: " << treatment_descriptor << std::endl;

  const std::string treatment_source{emp::keyname::pack({
    {"treat", treatment_descriptor},
    {"ext", ".tar.gz"},
  })};

  std::cout << "treatment_source: " << treatment_source << std::endl;

  emscripten_async_wget(
    treatment_source.c_str(),
    "treatment_source.tar.gz",
    treatget_callback,
    treatget_callback
  );

  return 0;
}
