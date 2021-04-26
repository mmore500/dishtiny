#include <deque>

#include <benchmark/benchmark.h>
#include <mpi.h>

#include "uitsl/debug/benchmark_utils.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/nonce/ScopeGuard.hpp"

#include "uit/setup/ImplSpec.hpp"

const uitsl::MpiGuard guard;

constexpr size_t buffer_size{ uit::DEFAULT_BUFFER };

static void MPI_Rsend(benchmark::State& state) {

  // set up
  std::deque<MPI_Request> requests;
  std::deque<int> buffers;
  size_t drop_counter{};
  size_t streak_counter{};
  size_t current_streak{};
  size_t epoch_counter{};


  // benchmark
  for (auto _ : state) {

    ++epoch_counter;

    // if send buffer is at capacity, make some space
    if (requests.size() == buffer_size) {

      // if front request is complete, pop it
      requests.pop_front();
      buffers.pop_front();

    }

    // add a send request
    requests.emplace_back();
    buffers.emplace_back();
    UITSL_Rsend(
      &buffers.back(), // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      1, // int dest
      1, // int tag
      MPI_COMM_WORLD // MPI_Comm comm
    );

  }


  // log results
  state.counters.insert({
    {
      "Processes",
      benchmark::Counter(
        uitsl::get_nprocs(),
        benchmark::Counter::kAvgThreads
      )
    }
  });

}

static void post_fresh_recvs(
  std::deque<MPI_Request>& requests,
  std::deque<int>& buffers
) {

  for (size_t i = 0; i < buffer_size; ++i) {
    requests.emplace_back();
    buffers.emplace_back();
    UITSL_Irecv(
      &buffers.back(), // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &requests.back() // MPI_Request * request
    );
  }

  if (requests.size() > 2 * buffer_size) {

    // testing requests before releasing them is necessary for MPICH
    emp::vector<MPI_Request> contiguous(
      std::begin(requests),
      std::prev(std::end(requests), 2 * buffer_size)
    );
    UITSL_Waitall(
      contiguous.size(),
      contiguous.data(),
      MPI_STATUSES_IGNORE
    );

    requests.erase(
      std::begin(requests),
      std::prev(std::end(requests), 2 * buffer_size)
    );

  }


}


// post continuous stream of receives to match incoming sends
static void support() {

  std::deque<MPI_Request> requests;
  std::deque<int> buffers;

  post_fresh_recvs(requests, buffers);

  // signal setup is complete
  UITSL_Barrier(MPI_COMM_WORLD);

  // this barrier will signal when benchmarking is complete
  MPI_Request ibarrier_request;
  UITSL_Ibarrier(MPI_COMM_WORLD, &ibarrier_request);

  // loop until benchmarking is complete
  while (!uitsl::test_completion(ibarrier_request)) {

    // has sender started to catch up with our posted recv's?
    if (uitsl::test_completion(requests[requests.size() - buffer_size])) {
      post_fresh_recvs(requests, buffers);
    }

  }

}

// register benchmark
const uitsl::ScopeGuard registration{[](){
  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Rsend",
      MPI_Rsend
    )
  );
}};

int main(int argc, char** argv) {

  // only root runs benchmark
  if (uitsl::is_root()) {

    benchmark::Initialize(&argc, argv);

    // wait for support to complete setup
    UITSL_Barrier(MPI_COMM_WORLD);

    benchmark::RunSpecifiedBenchmarks();

    // notify support that benchmarking is complete
    MPI_Request ibarrier_request;
    UITSL_Ibarrier(MPI_COMM_WORLD, &ibarrier_request);
    UITSL_Wait(&ibarrier_request, MPI_STATUSES_IGNORE);

  } else {

    support();

  }


}
