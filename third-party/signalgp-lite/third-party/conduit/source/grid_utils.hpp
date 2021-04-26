#pragma once

#include <algorithm>
#include <cstdlib>
#include <random>
#include <string>
#include <thread>
#include <vector>

#ifdef PP_USE_OMP
#include <omp.h>
#endif

#include "../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/concurrent/ConcurrentTimeoutBarrier.hpp"
#include "uitsl/concurrent/Gatherer.hpp"
#include "uitsl/countdown/Counter.hpp"
#include "uitsl/countdown/Timer.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/debug/safe_compare.hpp"
#include "uitsl/math/math_utils.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/parallel/ThreadIbarrierFactory.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/polyfill/barrier.hpp"
#include "uitsl/polyfill/latch.hpp"

#include "uit/fixtures/Conduit.hpp"

#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"

#include "chunk_utils.hpp"
#include "config_utils.hpp"
#include "State.hpp"
#include "Tile.hpp"


grid_t make_grid(const config_t & cfg) {

  emp::vector<uit::Inlet<Spec>> inlets;
  emp::vector<uit::Outlet<Spec>> outlets;

  const size_t grid_size = cfg.at("grid_size");
  const size_t num_threads = cfg.at("num_threads");
  netuit::Mesh<Spec> mesh{
    netuit::RingTopologyFactory{}(grid_size * uitsl::get_nprocs()),
    uitsl::AssignContiguously<uitsl::thread_id_t>{num_threads, grid_size}
  };

  grid_t grid;

  netuit::Mesh<Spec>::submesh_t submesh{ mesh.GetSubmesh() };

  for (const auto & node : submesh) {
    grid.push_back(
      Tile(
        node.GetInput(0),
        node.GetOutput(0)
      )
    );
  }

  for (size_t i = 0; i < grid_size; ++i) {
    grid[i].next = &grid[uitsl::circular_index(i, grid_size , 1)];
    grid[i].prev = &grid[uitsl::circular_index(i, grid_size, -1)];
    grid[i].id = i;
  }


  return grid;

}

void initialize_grid(grid_t & grid) {

  const std::array<State, 2> states{'_', 'O'};

  for (size_t i = 0; i < grid.size(); ++i) {
    grid[i].SetState(states[i % states.size()]);
  }

}

double run_grid(grid_t & grid, const config_t & cfg) {

  emp::vector<chunk_t> chunks(
    make_chunks(
      grid,
      cfg.at("num_chunks")
    )
  );

  initialize_grid(grid);

  const size_t num_updates = cfg.at("num_updates");
  const size_t num_seconds = cfg.at("num_seconds");
  const size_t verbose = cfg.at("verbose");
  const size_t resistance = cfg.at("resistance");
  const size_t num_threads = cfg.at("num_threads");
  const size_t use_omp = cfg.at("use_omp");
  const size_t synchronous = cfg.at("synchronous");
  const size_t shuffle_tile_evaluation = cfg.at("shuffle_tile_evaluation");
  const size_t checkout_memory = cfg.at("checkout_memory");

  const auto task_step = [=](chunk_t chunk){
    update_chunk(chunk, verbose, shuffle_tile_evaluation, resistance);
  };

  std::latch latch{uitsl::safe_cast<std::ptrdiff_t>(num_threads)};
  std::barrier barrier{uitsl::safe_cast<std::ptrdiff_t>(num_threads)};
  uitsl::ThreadIbarrierFactory factory{ num_threads };

  uitsl::Gatherer<int> gatherer(MPI_INT);

  const auto task_sequence = [&](chunk_t source){

    emp_assert(!use_omp);

    auto chunk = checkout_memory
      ? checkout_chunk(source)
      : source;

    uitsl::Timer<std::chrono::seconds, uitsl::CoarseClock> timer{
      std::chrono::seconds{num_seconds}
    };
    uitsl::Counter counter{num_updates};

    // synchronize once after thread creation and MPI spinup
    if (!synchronous) {
      latch.arrive_and_wait();
      MPI_Barrier(MPI_COMM_WORLD);
    }

    while (!counter.IsComplete() && !timer.IsComplete()) {
      task_step(chunk);

      // synchronize after each step
      if (synchronous) {
        const uitsl::ThreadIbarrier thread_barrier{ factory.MakeBarrier() };
        uitsl::ConcurrentTimeoutBarrier{
          thread_barrier,
          timer
        };
      }

      counter.Step();

    }

    if (checkout_memory) checkin_chunk(source, chunk);

    gatherer.Put(uitsl::safe_cast<int>(
      counter.GetElapsed() / (
        num_seconds
        ?: std::chrono::duration_cast<std::chrono::duration<double>>(
          timer.GetElapsed()
        ).count() ?: 1
      )
    ));

  };

  const auto omp_sync = [task_step, num_updates, &chunks](){
    #ifdef PP_USE_OMP
    const size_t num_chunks = chunks.size();

    for (size_t update = 0; update < num_updates; ++update) {
        #pragma omp parallel for
        for (size_t i = 0; i < num_chunks; ++i) task_step(chunks[i]);
      }
    }
    #endif
  };

  const auto omp_async = [task_step, &chunks](){
    #ifdef PP_USE_OMP
    const size_t num_chunks = chunks.size();

    #pragma omp parallel
    {
      // attempt to ensure synchonous thread initialization
      #pragma omp barrier

      #pragma omp for
      for (size_t i = 0; i < num_chunks; ++i) task_step(chunks[i]);
    }
    #endif
  };

  const auto std_run = [&](){
    uitsl::ThreadTeam team;

    for (auto chunk : chunks) {
      team.Add([chunk, task_sequence](){ task_sequence(chunk); });
    }

    team.Join();

  };

  if (use_omp) {
    if (synchronous) omp_sync();
    else omp_async();
  } else std_run();

  const auto productivities = gatherer.Gather();

  return productivities
    ? std::accumulate(
      std::begin(*productivities),
      std::end(*productivities),
      0.0
    ) / productivities->size()
    : -1.0;

}

void audit_grid(
  const grid_t & grid,
  const config_t& cfg,
  const size_t nanoseconds
) {

  std::unordered_map<std::string, std::string> descriptors{
    {"title", "audit"},
    {"nanoseconds", emp::to_string(nanoseconds)},
    {"ext", ".csv"}
  };

  std::transform(
    std::begin(cfg),
    std::end(cfg),
    std::inserter(descriptors, std::begin(descriptors)),
    [](const auto & pair){
      const auto & [key, value] = pair;
      return std::pair<std::string, std::string>{key, emp::to_string(value)};
    }
  );

  emp::DataFile datafile(
    emp::keyname::pack(descriptors)
  );

  size_t tile;
  datafile.AddVar<size_t>(tile, "Tile");

  size_t attempted_write_count;
  datafile.AddVar<size_t>(attempted_write_count, "Attempted Write Count");

  size_t blocked_write_count;
  datafile.AddVar<size_t>(blocked_write_count, "Blocked Write Count");

  size_t dropped_write_count;
  datafile.AddVar<size_t>(dropped_write_count, "Dropped Write Count");

  size_t read_count;
  datafile.AddVar<size_t>(read_count, "Read Count");

  size_t read_revision_count;
  datafile.AddVar<size_t>(read_revision_count, "Read Revision Count");

  size_t net_flux;
  datafile.AddVar<size_t>(net_flux, "Net Flux");

  datafile.PrintHeaderKeys();

  for (tile = 0; tile < grid.size(); ++tile) {
    const auto & which = grid[tile];

    attempted_write_count = which.GetAttemptedPutCount();
    blocked_write_count = which.GetBlockedPutCount();
    dropped_write_count = which.GetDroppedPutCount();
    read_count = which.GetReadCount();
    read_revision_count = which.GetReadRevisionCount();
    net_flux = which.GetNetFlux();

    datafile.Update();
  }



}
