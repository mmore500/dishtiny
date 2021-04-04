#include <cstdlib>

#include "conduit/include/uitsl/countdown/ProgressBar.hpp"
#include "conduit/include/uitsl/countdown/Timer.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"
#include "conduit/include/uitsl/parallel/ThreadTeam.hpp"
#include "Empirical/include/emp/config/ArgManager.hpp"
#include "Empirical/include/emp/config/command_line.hpp"
#include "Empirical/include/emp/math/Random.hpp"

#include "dish2/config/cfg.hpp"
#include "dish2/config/thread_idx.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const uitsl::MpiGuard guard;

using spec_t = sgpl::Spec<>;

using timekeeper_t = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;

using bar_t = uitsl::ProgressBar<timekeeper_t>;

using Spec = dish2::Spec_default;

void fuzz_one() {

  dish2::ProcWorld<Spec> proc_world;

  uitsl::ThreadTeam team;

  for (size_t tid{}; tid < dish2::cfg.N_THREADS(); ++tid) {
    team.Add( [&proc_world, tid](){
      dish2::thread_idx = tid;
      auto thread_world = proc_world.MakeThreadWorld();
      for ( [[maybe_unused]] auto _ : timekeeper_t{ std::chrono::seconds{ 30 } }
      ) thread_world.Update();
    } );
  }

  team.Join();

}

int main(int argc, char *argv[]) {

  emp::ArgManager am(
    argc,
    argv,
    emp::ArgManager::make_builtin_specs(&dish2::cfg)
  );

  am.UseCallbacks();
  if (am.HasUnused()) std::exit( EXIT_FAILURE );

  for ([[maybe_unused]] auto i : bar_t{ std::chrono::minutes{ 10 } })
    fuzz_one();

}
