// This is the main function for the NATIVE version of this project.

#include "tools/Random.h"
#include "config/command_line.h"
#include "config/ArgManager.h"

#include "../CustomConfig.h"
#include "../DishtinyConfig.h"
#include "../GridSpec.h"
#include "../TinyWorld.h"


int main(int argc, char* argv[])
{

  // Read configs.
  std::string config_fname = "configs.cfg";
  auto args = emp::cl::ArgManager(argc, argv);
  DishtinyConfig dconfig;
  dconfig.Read(config_fname);

  if (args.ProcessConfigOptions(dconfig, std::cout, config_fname, "../DishtinyConfig.h") == false)
    exit(0);
  if (args.TestUnknown() == false)
    exit(0);

  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  dconfig.Write(std::cout);
  std::cout << "==============================\n"
            << std::endl;


  GridSpec spec(dconfig);
  CustomConfig cconfig(dconfig, spec);
  emp::Random rand(dconfig.SEED());

  TinyWorld world(dconfig.RUN_LENGTH(), rand, dconfig, cconfig);
  world.Run();

  return 0;
}
