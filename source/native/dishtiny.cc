// This is the main function for the NATIVE version of this project.

#include "tools/Random.h"
#include "config/command_line.h"
#include "config/ArgManager.h"

#include "../Config.h"
#include "../DishWorld.h"
#ifndef NDATA
#include "../DataHelper.h"
#endif

#include "../Config.cpp"
#include "../DishWorld.cpp"
#include "../FrameHardware.cpp"
#include "../FrameCell.cpp"
#include "../Manager.cpp"


int main(int argc, char* argv[])
{

  std::cout << "SOURCE HASH " << STRINGIFY(DISHTINY_HASH_) << std::endl;
  std::cout << "EMP HASH " << STRINGIFY(EMPIRICAL_HASH_) << std::endl;
  emp::Random temp(1); // need this to prevent a memory leak
  std::cout << "MATCHBIN "
    << Config::hardware_t(nullptr, nullptr, &temp).GetMatchBin().name()
    << std::endl;

  Config cfg;
  cfg.LoadFromFile();
  const auto specs = emp::ArgManager::make_builtin_specs(&cfg);
  emp::ArgManager am(argc, argv, specs);
  am.UseCallbacks();
  if (am.HasUnused()) std::exit(EXIT_FAILURE);

  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  cfg.WriteMe(std::cout);
  std::cout << "==============================\n" << std::endl;


  DishWorld world(cfg);
  #ifndef NDATA
  DataHelper datahelper(world, cfg);
  #endif

  for(size_t ud = 0; ud < cfg.RUN_LENGTH(); ++ud) {
    if (ud % 20 == 0) std::cout << "Update: " << ud << std::endl;
    world.Update();
  }
  // so that our computations won't get optimized away
  std::cout << world.GetNumOrgs() << std::endl;

  return 0;
}
