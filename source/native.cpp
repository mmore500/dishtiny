// This is the main function for the NATIVE version of this project.

#include <cstdlib>

#include "conduit/include/uitsl/chrono/CoarseClock.hpp"
#include "conduit/include/uitsl/countdown/Timer.hpp"
#include "conduit/include/uitsl/debug/safe_cast.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"
#include "conduit/include/uitsl/parallel/ThreadTeam.hpp"
#include "Empirical/include/emp/base/macros.hpp"
#include "Empirical/include/emp/config/ArgManager.hpp"
#include "Empirical/include/emp/meta/meta.hpp"
#include "signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "dish2/introspection/write_summary_metrics.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const uitsl::MpiGuard mpi_guard;

using Spec = dish2::Spec;

void setup_config( int argc, char* argv[] ) {

  std::cout << "SOURCE HASH " << EMP_STRINGIFY(DISHTINY_HASH_) << std::endl;

  dish2::cfg.Read( "config.cfg" );
  const auto specs = emp::ArgManager::make_builtin_specs(&dish2::cfg);
  emp::ArgManager arg_manager{ argc, argv, specs };
  arg_manager.UseCallbacks();
  if ( arg_manager.HasUnused() ) std::exit(EXIT_FAILURE);

  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;

  dish2::cfg.Write( std::cout );

  std::cout << "==============================\n" << std::endl;

}

void thread_job(
  const size_t thread, dish2::ThreadWorld<Spec> thread_world
) {

  // setup thread local ranodm
  sgpl::ThreadLocalRandom::Get() = emp::Random( uitsl::safe_cast<int>(
    1 +
    emp::CombineHash( thread, dish2::cfg.RNG_SEED() )
      % ( std::numeric_limits<int>::max() - 1 )
  ) );

  uitsl::Timer<
    std::chrono::duration<double, std::ratio<1>>,
    uitsl::CoarseClock
  > run_timer{ std::chrono::duration<double, std::ratio<1>>{
    dish2::cfg.RUN_SECONDS() ?: std::numeric_limits<double>::infinity()
  } };

  uitsl::Timer<
    std::chrono::duration<double, std::ratio<1>>,
    uitsl::CoarseClock
  > log_timer{ std::chrono::duration<double, std::ratio<1>>{
    dish2::cfg.LOG_FREQ()
  } };

  while (
    !run_timer.IsComplete()
    && thread_world.GetUpdate()
      < ( dish2::cfg.RUN_UPDATES() ?: std::numeric_limits<size_t>::max() )
   ) {

    if ( log_timer.IsComplete() ) {
      log_timer = decltype(log_timer){
        std::chrono::duration<double, std::ratio<1>>{ dish2::cfg.LOG_FREQ() }
      };
      std::cout << "thread " << thread;
      std::cout << " @ update " << thread_world.GetUpdate();
      if ( dish2::cfg.RUN_UPDATES() ) {
        std::cout << " of " << dish2::cfg.RUN_UPDATES()
          << " ("
          << 100.0 * thread_world.GetUpdate() / dish2::cfg.RUN_UPDATES()
          << "%)";
      }
      std::cout << " + second " << run_timer.GetElapsed().count();
      if ( dish2::cfg.RUN_SECONDS() ) {
        std::cout << " of " << dish2::cfg.RUN_SECONDS()
        << " (" << 100.0 * run_timer.GetFractionComplete() << "%)";
      }
      std::cout << std::endl;
    }

    // update the simulation
    thread_world.Update();

  }

  dish2::write_summary_metrics<Spec>( thread_world, thread );

}

int main(int argc, char* argv[]) {

  setup_config(argc, argv);

  uitsl::ThreadTeam team;

  dish2::ProcWorld<Spec> proc_world;

  for( size_t thread{}; thread < dish2::cfg.N_THREADS(); ++thread ) team.Add(
    [thread, &proc_world](){ thread_job(
        thread, proc_world.MakeThreadWorld( thread )
    ); }
  );

  team.Join();

  return 0;

}
