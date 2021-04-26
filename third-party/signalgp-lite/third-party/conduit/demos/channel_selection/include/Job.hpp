#pragma once

#include <fstream>
#include <limits>
#include <sstream>
#include <string>

#include "../third-party/Empirical/include/emp/io/NullStream.hpp"

#include "uitsl/chrono/ClockDeltaDetector.hpp"
#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/concurrent/ConcurrentBarrier.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"
#include "uitsl/mpi/comm_utils.hpp"
#include "uitsl/parallel/ThreadIbarrierFactory.hpp"
#include "uitsl/polyfill/latch.hpp"

#include "CellCollection.hpp"
#include "config/cfg.hpp"

class Job {

  CellCollection collection;

  using timer_t = uitsl::CoarseTimer;
  using bar_t = uitsl::ProgressBar<timer_t>;

  size_t update_counter{};

  uitsl::ClockDeltaDetector<> delta_sync;

public:

  Job(const size_t thread_idx, const submesh_t& submesh)
  : collection(submesh)
  {

    // initialized first time thru the function,
    // so N_THREADS should be initialized
    static uitsl::ConcurrentBarrier barrier1{
      cfg.N_THREADS(), uitsl::duplicate_comm( MPI_COMM_WORLD )
    };
    static uitsl::ConcurrentBarrier barrier2{
      cfg.N_THREADS(), uitsl::duplicate_comm( MPI_COMM_WORLD )
    };

    const bool use_intra = ( cfg.ASYNCHRONOUS() != 4 );
    const bool is_multiproc = uitsl::is_multiprocess();

    // for a fair profile, don't start the benchmark
    // until all procs, threads are ready
    barrier1.ArriveAndWait();

    // only use progress bar for single process jobs
    // or first thread of multithread jobs
    bar_t timer{
      uitsl::is_root() && thread_idx == 0 ? std::cout : emp::nout,
      cfg.RUN_SECONDS() ?: std::numeric_limits<double>::infinity()
    };
    uitsl::CoarseRealTimer timer_sync{ std::chrono::milliseconds{ 10 } };

    for ( const auto __ : timer ) { // begin benchmarking loop
      ++update_counter;
      collection.Update(use_intra);

      if ( !cfg.ASYNCHRONOUS() ) {
        barrier1.ArriveAndWaitWhile( timer );
      } else if (
        is_multiproc && cfg.ASYNCHRONOUS() == 1 && timer_sync.IsComplete()
      ) {
        // intermittently have procs clear out pending input
        barrier1.ArriveAndWaitWhile( timer );
        collection.PullInputs();
        barrier1.ArriveAndWaitWhile( timer );
        timer_sync.Reset();
      } else if (
        is_multiproc && cfg.ASYNCHRONOUS() == 2 && delta_sync.HasDeltaElapsed()
      ) {
        // intermittently have procs clear out pending input
        barrier1.ArriveAndWaitWhile( timer );
        collection.PullInputs();
        barrier1.ArriveAndWaitWhile( timer );
      } else {
        // nop for cfg.ASYNCHRONOUS() == 3 and 4
      }
    } // end benchmarking loop
    std::cout << "." << std::flush;

    barrier2.ArriveAndWait();
    if ( uitsl::is_root() && thread_idx == 0 ) {
      std::cout << " all benchmarking loops complete" << '\n';
    }

    // dump data
    std::ofstream( emp::keyname::pack({
      {"a", "updates_elapsed"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".txt"},
    }) ) << update_counter << '\n';

    std::ofstream( emp::keyname::pack({
      {"a", "num_messages_sent"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".txt"},
    }) ) << collection.GetNumMessagesSent() << '\n';

    std::ofstream( emp::keyname::pack({
      {"a", "num_messages_received"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".txt"},
    }) ) << collection.GetNumMessagesReceived() << '\n';

    if ( cfg.ASYNCHRONOUS() ) {
      // try to ensure consistent reading for num_conflicts
      collection.PullInputs();
      collection.PushOutputs();
      barrier2.ArriveAndWait();
    }

    // base number of conflicts on the final state of the simulation
    collection.PullInputs();

    std::ofstream( emp::keyname::pack({
      {"a", "num_conflicts"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".txt"},
    }) ) << collection.CountConflicts() << '\n';

  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "job size " << collection.GetSize() << '\n';
    ss << "updates elapsed " << update_counter << '\n';
    ss << "num messages sent " << collection.GetNumMessagesSent() << '\n';
    ss << "num messages received " << collection.GetNumMessagesReceived()
      << '\n';
    ss << "num chanel conflicts " << collection.CountConflicts()
      << '\n';
    return ss.str();
  }

};
