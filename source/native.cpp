// This is the main function for the NATIVE version of this project.

#include <cstdlib>
#include <chrono>

#include <omp.h>

#include "cereal/include/cereal/cereal.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "cereal/include/cereal/types/array.hpp"
#include "cereal/include/cereal/types/vector.hpp"
#include "cereal/include/cereal/types/map.hpp"
#include "cereal/include/cereal/types/unordered_map.hpp"
#include "cereal/include/cereal/types/string.hpp"
#include "Empirical/source/tools/Random.h"
#include "Empirical/source/config/command_line.h"
#include "Empirical/source/config/ArgManager.h"

#include "dishtiny/Config.hpp"
#include "dishtiny/DishWorld.hpp"
#ifndef NDATA
#include "dishtiny/DataHelper.hpp"
#endif

#include "dishtiny/Config.cpp"
#include "dishtiny/DishWorld.cpp"
#include "dishtiny/FrameHardware.cpp"
#include "dishtiny/FrameCell.cpp"
#include "dishtiny/Manager.cpp"
#include "dishtiny/LibraryInstruction.cpp"
#include "dishtiny/LibraryInstructionSpiker.cpp"
#include "dishtiny/ManagerConnection.cpp"

int main(int argc, char* argv[]) {

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


  #pragma omp parallel
  {
    #pragma omp single
    {

      std::cout
        << "omp_get_num_threads() "
        << omp_get_num_threads()
        << std::endl;
    }
  }

  DishWorld world(cfg);
  #ifndef NDATA
  DataHelper datahelper(world, cfg);
  #endif


  const auto begin_time = (
    std::chrono::steady_clock::now()
    - std::chrono::seconds{[](){
      if (const char* SECONDS = std::getenv("SECONDS"); SECONDS) {
        return std::stoi(SECONDS);
      } else return 0;
    }()}
  );

  auto is_time_expired = [&cfg, begin_time](){

    return (
      cfg.RUN_SECONDS() // if RUN_SECONDS is zero, never expire
      && (
        std::chrono::steady_clock::now() - begin_time
        > std::chrono::seconds{cfg.RUN_SECONDS()}
      )
    );

  };

  size_t expiration_update = 0;

  while (true) {

    // print an intermittent update message
    if (world.GetUpdate() % 32 == 0) {
      std::cout << world.GetUpdate() << std::endl;
    }

    // check to see if the run is expired (time or update count)
    if (
      const bool is_expired = (
        // only check time every 32 updates
        (world.GetUpdate() % 32 == 0 && is_time_expired())
        || (cfg.RUN_LENGTH() && world.GetUpdate() >= cfg.RUN_LENGTH())
      );
      // if is expired but also not already expired
      is_expired && !expiration_update
    ) expiration_update = world.GetUpdate();

    // do one last snapshot before expiring
    if (
      expiration_update
      && world.GetUpdate() >= expiration_update + cfg.SNAPSHOT_LENGTH()
    ) break;

    // perform snapshot components during run or right after it is expired
    #ifndef NDATA
    if (
      // if we want to do a regular snapshot...
      (cfg.SNAPSHOT_FREQUENCY()
        && world.GetUpdate()
        && world.GetUpdate() % cfg.SNAPSHOT_FREQUENCY() == 0
      )
      // ... or if we are wrapping up after expiring
      || (expiration_update
        && world.GetUpdate() == expiration_update)
    ) datahelper.SnapshotPopulation();

    if (
      // if we want to do a regular snapshot...
      (cfg.SNAPSHOT_FREQUENCY()
        && world.GetUpdate() % cfg.SNAPSHOT_FREQUENCY() < cfg.SNAPSHOT_LENGTH()
      )
      // ... or if we are wrapping up after expiring
      || expiration_update
    ) datahelper.SnapshotPhenotypes();
    #endif

    // update the simulation
    world.Update();

  }

  std::cout << "EXPIRATION_UPDATE " << expiration_update << std::endl;

  // so that our computations won't get optimized away
  std::cout << world.GetNumOrgs() << std::endl;

  return 0;
}
