#include <string>
#include <unistd.h>

#include <emscripten.h>
#include <zlib.h>

#include "conduit/include/uitsl/debug/list_cwd.hpp"
#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "conduit/include/uitsl/utility/inflate.hpp"
#include "conduit/include/uitsl/utility/untar.hpp"
#include "Empirical/source/config/ArgManager.h"
#include "Empirical/source/config/command_line.h"
#include "Empirical/source/tools/keyname_utils.h"
#include "Empirical/source/web/UrlParams.h"

#include "dish2/config/cfg.hpp"
#include "dish2/web/WebInterface.hpp"
#include "dish2/world/ProcWorld.hpp"

thread_local dish2::WebInterface* interface;

void run() {

  // apply configuration query params and config files to Config
  auto specs = emp::ArgManager::make_builtin_specs( &dish2::cfg );
  emp::ArgManager am( emp::web::GetUrlParams(), specs );
  // cfg.LoadFromFile();
  am.UseCallbacks();
  if (am.HasUnused()) std::exit(EXIT_FAILURE);

  interface = new dish2::WebInterface;

  // log configuraiton settings
  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  // cfg.WriteMe(std::cout);
  std::cout << "==============================\n" << std::endl;


  // set up web interface
  interface->Redraw();

  std::cout << "web viewer load SUCCESS" << std::endl;

  // once we're done setting up, turn off the loading modal
  MAIN_THREAD_EM_ASM({ $('.modal').modal('hide'); });

}

// unpack treatment directory and jump in there, then call run
void treatget_callback(const char *filename) {

  uitsl::untar( uitsl::inflate( filename ) );

  std::filesystem::remove(filename);

  std::cout << "changing directory..." << std::endl;
  chdir("treatment_directory");
  uitsl::list_cwd();

  run();

}

int main() {

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

  // emscripten_async_wget(
  //   treatment_source.c_str(),
  //   "treatment_source.tar.gz",
  //   treatget_callback,
  //   treatget_callback
  // );

  run();

  emscripten_set_main_loop([](){}, 1, true);

  return 0;
}
