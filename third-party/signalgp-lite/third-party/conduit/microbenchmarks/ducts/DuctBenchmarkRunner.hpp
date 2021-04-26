#pragma once

#include <assert.h>
#include <optional>
#include <ratio>
#include <thread>
#include <tuple>

#include <benchmark/benchmark.h>
#include <mpi.h>

#include "uitsl/chrono/TimeGuard.hpp"
#include "uitsl/concurrent/Gatherer.hpp"
#include "uitsl/debug/benchmark_utils.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/nonce/CircularIndex.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/parallel/thread_utils.hpp"

#include "uit/fixtures/Conduit.hpp"

#include "netuit/mesh/Mesh.hpp"

template<
  typename NumThreadsType,
  typename MeshFactory,
  typename ImplSpec
>
struct DuctMicrobenchRunner {

  using MESSAGE_T = typename ImplSpec::T;
  using submesh_t = typename netuit::Mesh<ImplSpec>::submesh_t;

  netuit::Mesh<ImplSpec> mesh{
    MeshFactory{}(NumThreadsType{}() * uitsl::get_nprocs()),
    [](const uitsl::thread_id_t tid) {
      // single proc: all nodes assigned to unique thread
      // multi proc: all nodes assigned to thread 0
      return uitsl::is_multiprocess()
        ? uitsl::AssignIntegrated<uitsl::thread_id_t>{}(tid)
        : uitsl::AssignSegregated<uitsl::thread_id_t>{}(tid)
      ;
    },
    [](const uitsl::proc_id_t pid) {
      // single proc: all nodes assigned to proc 0
      // multi proc: all nodes assigned to different proc
      return uitsl::is_multiprocess()
        ? uitsl::AssignSegregated<uitsl::proc_id_t>{}(pid)
        : uitsl::AssignIntegrated<uitsl::proc_id_t>{}(pid)
      ;
    }
  };

  // infrastructure to gather latency readings from support processes
  uitsl::Gatherer<long long int> latency_gatherer{MPI_INT};

  struct StatTracker {
    // how many send/receive cycles elapsed during the benchmark?
    // (incremented during each send/receive cycle)
    int epoch_counter{};

    // how many unique msgs have we received?
    // (incremented if current GetCurernt differs from prev JumpGet)
    size_t unique_msg_counter{};

    // sum of differences between sequence stamps of sent
    // and received messages
    long long int net_latency{};


  };

  void Run(benchmark::State& state) {

    // only root process runs benchmark
    // other procs just pump their conduits until benchmark concludes
    if (uitsl::is_root()) {
      auto [last_sent_msg, res] = DoBenchmark(state);
      LogResult(state, res);
    } else {
      DoSupport(state);
    }

  }

  decltype(auto) DoBenchmark(benchmark::State& state) {

    // setup
    submesh_t submesh { mesh.GetSubmesh(state.thread_index) };

    MESSAGE_T previously_received_msg{};
    MESSAGE_T previously_sent_msg{};
    StatTracker res;

    // wait for support to complete setup
    if (state.thread_index == 0) UITSL_Barrier(MPI_COMM_WORLD);

    // benchmark
    for (const auto _ : state) {

      MESSAGE_T current_received_msg{};

      // pump all the conduits once
      for (auto& node : submesh) {
        for (auto& output : node.GetOutputs()) {
          output.TryPut(previously_sent_msg);
        }
        for (auto& input : node.GetInputs()) {
          current_received_msg = input.JumpGet();
        }
      }
      ++previously_sent_msg;

      // data collection
      res.unique_msg_counter += current_received_msg != previously_received_msg;
      res.net_latency += previously_sent_msg - previously_received_msg;
      res.epoch_counter++;

      // safety checks in debug mode
      if (submesh.size() && submesh.front().GetNumInputs()) emp_assert(
        current_received_msg >= 0, "Messages should be positive."
      );
      if (submesh.size() && submesh.front().GetNumInputs()) emp_assert(
        previously_received_msg <= current_received_msg
        || current_received_msg == 0
        "Messages should be monotonicaly increasing."
      );

      previously_received_msg = current_received_msg;
    }

    // notify support that benchmarking is complete
    if (state.thread_index == 0) {
      MPI_Request ibarrier_request;
      UITSL_Ibarrier(MPI_COMM_WORLD, &ibarrier_request);
      UITSL_Wait(&ibarrier_request, MPI_STATUSES_IGNORE);
    }

    return std::tuple{previously_sent_msg, res};


  };

  void LogResult(benchmark::State& state, StatTracker& res) {

    // gather latency measurements from other procs
    // TODO the multiproc latency measurement seems broken
    if (uitsl::is_multiprocess()) {
      latency_gatherer.Put(res.net_latency);
      const auto gathered = latency_gatherer.Gather();
      if (gathered) {
        res.net_latency = std::accumulate(
          std::begin(*gathered),
          std::end(*gathered),
          0.0
        ) / gathered->size();
      }
    }

    state.counters.insert({
      {
        "Net Latency",
        benchmark::Counter(
          res.net_latency,
          benchmark::Counter::kAvgThreads
        )
      },
      {
        "Number Unique Messages",
        benchmark::Counter(
          res.unique_msg_counter,
          benchmark::Counter::kAvgThreads
        )
      },
      {
        "Number Epochs",
        benchmark::Counter(
          res.epoch_counter,
          benchmark::Counter::kAvgThreads
        )
      },
      {
        "Latency",
        benchmark::Counter(
          res.net_latency * res.unique_msg_counter
            / static_cast<double>(res.epoch_counter),
          benchmark::Counter::kAvgThreadsRate | benchmark::Counter::kInvert
        )
      },
      {
        "Lossiness",
        benchmark::Counter(
          1.0 - res.unique_msg_counter / static_cast<double>(res.epoch_counter),
          benchmark::Counter::kAvgThreads
        )
      },
      {
        "Processes",
        benchmark::Counter(
          uitsl::get_nprocs(),
          benchmark::Counter::kAvgThreads
        )
      }
    });
  }

  void DoSupport(benchmark::State& state) {

    // setup
    MESSAGE_T previously_received_msg{};
    MESSAGE_T previously_sent_msg{};
    long long int net_latency{};
    submesh_t submesh { mesh.GetSubmesh(state.thread_index) };

    // signal setup is complete
    UITSL_Barrier(MPI_COMM_WORLD);

    // this barrier will signal when benchmarking is complete
    MPI_Request ibarrier_request;
    UITSL_Ibarrier(MPI_COMM_WORLD, &ibarrier_request);

    // loop until benchmarking is complete
    while (!uitsl::test_completion(ibarrier_request)) {

      // check if benchmarking complete intermittently
      for (size_t i = 0; i < 100; ++i) {

        MESSAGE_T current_received_msg{};

        // pump all the dconduits once
        for (auto& node : submesh) {
          for (auto& output : node.GetOutputs()) {
            output.TryPut(previously_sent_msg);
          }
          for (auto& input : node.GetInputs()) {
            current_received_msg = input.JumpGet();
          }
        }
        ++previously_sent_msg;

        // collect latency data
        net_latency += previously_sent_msg - previously_received_msg;

        previously_received_msg = current_received_msg;
      }

    }

    // report net latency to root process
    latency_gatherer.Put(net_latency);
    latency_gatherer.Gather();

    // report that this process isn't running a valid benchmark
    state.SkipWithError("is support, not benchmark");

  }

};
