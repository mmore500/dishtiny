// This is the main function for the NATIVE version of this project.

#include "tools/Random.h"
#include "config/command_line.h"
#include "config/ArgManager.h"

#include "../Config.h"
#include "../DishWorld.h"


int main(int argc, char* argv[])
{

  // Read configs.
  std::string config_fname = "configs.cfg";
  Config cfg(config_fname, emp::cl::ArgManager(argc, argv));

  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  cfg.WriteMe(std::cout);
  std::cout << "==============================\n"
            << std::endl;


  DishWorld world(cfg);
  // world.Run();

  return 0;
}
