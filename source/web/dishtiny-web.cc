#include <string>
#include <unistd.h>
#include <experimental/filesystem>

#include <emscripten.h>
#include <zlib.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>

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
#include "../LibraryInstructionSpiker.cpp"
#include "../ManagerConnection.cpp"

#include "inflate.h"
#include "untar.h"


Config cfg;

// for logging
void pwd() {
  std::cout << std::experimental::filesystem::absolute(".") << std::endl;
  for (auto& p: std::experimental::filesystem::directory_iterator(".")) {
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

  // once we're done setting up, turn off the loading modal
  emscripten_run_script("$('.modal').modal('hide');");
}

// unpack treatment directory and jump in there, then call run
void treatget_callback(const char *filename) {

  auto file = gzopen(filename, "rb");
  auto temp = std::tmpfile();

  inflate(file, temp);

  gzclose(file);

  std::rewind(temp);

  untar(temp, "temp");

  std::fclose(temp);

  std::experimental::filesystem::remove(filename);

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

  const std::string treatment_source{emp::to_string(
    "treat=",
    treatment_descriptor,
    ".tar.gz"
  )};

  std::cout << "treatment_source: " << treatment_source << std::endl;

  emscripten_async_wget(
    treatment_source.c_str(),
    "treatment_source.tar.gz",
    treatget_callback,
    treatget_callback
  );

  return 0;
}
