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

static void MPI_Test_nopending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int buffer{};

  UITSL_Irecv(
    &buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  );

  // benchmark
  for (auto _ : state) {

    int flag{};

    UITSL_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
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

  // clean up
  UITSL_Cancel(&request);

}

static void MPI_Test_onepending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int recv_buffer{};
  int send_buffer{};

  UITSL_Irecv(
    &recv_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  );

  UITSL_Send(
    &send_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int dest
    1, // int tag
    MPI_COMM_WORLD // MPI_Comm comm
  );

  // benchmark
  for (auto _ : state) {

    int flag{};

    UITSL_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    );

  }

  // clean up
  UITSL_Wait(&request, MPI_STATUS_IGNORE);

}

static void MPI_Test_manypending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int recv_buffer{};
  int send_buffer{};

  UITSL_Irecv(
    &recv_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  );

  for (size_t i = 0; i < uit::DEFAULT_BUFFER; ++i) {
    MPI_Request send_request;
    UITSL_Isend(
      &send_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &send_request // MPI_Request * request
    );
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    UITSL_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    );

  }

  // clean up
  for (size_t i = 0; i < uit::DEFAULT_BUFFER - 1; ++i) {
    UITSL_Irecv(
      &recv_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &request // MPI_Request * request
    );


    UITSL_Wait(&request, MPI_STATUS_IGNORE);
  }

}

// register benchmarks
const uitsl::ScopeGuard registration{[](){
  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Test_nopending",
      MPI_Test_nopending
    )
  );

  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Test_onepending",
      MPI_Test_onepending
    )
  );

  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Test_manypending",
      MPI_Test_manypending
    )
  );
}};

int main(int argc, char** argv) {

  // only root runs benchmark
  if (uitsl::is_root()) {

    benchmark::Initialize(&argc, argv);

    benchmark::RunSpecifiedBenchmarks();

  }

}
