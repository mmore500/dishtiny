// This is the main function for the NATIVE version of this project.

#include <chrono>
#include <iostream>

#include <mpi.h>

#include "conduit/include/uitsl/mpi/comm_utils.hpp"
#include "conduit/include/uitsl/parallel/ThreadTeam.hpp"

#include "dish2/config/cfg.hpp"
#include "dish2/config/make_arg_specs.hpp"
#include "dish2/config/setup.hpp"
#include "dish2/config/thread_idx.hpp"
#include "dish2/debug/backtrace_enable.hpp"
#include "dish2/record/global_records_finalize.hpp"
#include "dish2/record/global_records_initialize.hpp"
#include "dish2/run/setup_thread_local_random.hpp"
#include "dish2/run/thread_job.hpp"
#include "dish2/spec/print_spec.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/utility/try_with_timeout.hpp"
#include "dish2/world/ProcWorld.hpp"

using Spec = DISH2_SPEC;

void do_main() {

  if ( uitsl::is_root() ) dish2::print_spec<Spec>();
  dish2::global_records_initialize();

  dish2::ProcWorld<Spec> proc_world;

  // synchronize all processes before entering simulation loop
  MPI_Barrier( MPI_COMM_WORLD );

  uitsl::ThreadTeam team;

  // launch threads to run simulation
  for ( size_t thread{}; thread < dish2::cfg.N_THREADS(); ++thread ) team.Add(
    [&proc_world, thread](){
      dish2::thread_idx = thread;
      dish2::setup_thread_local_random();
      dish2::thread_job<Spec>( proc_world.MakeThreadWorld() );
    }
  );

  // wait for threads to complete
  team.Join();

  dish2::global_records_finalize();

  std::cout << "process " << uitsl::get_proc_id() << " complete" << '\n';

}

int main(int argc, char* argv[]) {

  dish2::setup<Spec>(
    emp::ArgManager{ argc, argv, dish2::make_arg_specs<Spec>() }
  );

  const bool res = dish2::try_with_timeout(
    do_main,
    std::chrono::duration<double>( dish2::cfg.MAIN_TIMEOUT_SECONDS() )
  );
  if ( res ) return 0;
  else {
    std::cout << "process " << uitsl::get_proc_id() << " main timeout" << '\n';
    return 1;
  }

}
