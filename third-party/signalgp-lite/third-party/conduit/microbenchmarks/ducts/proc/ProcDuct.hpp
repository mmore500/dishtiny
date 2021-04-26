#include "uitsl/meta/ForEach.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/nonce/ScopeGuard.hpp"

#include "../DuctBenchmarkRegistration.hpp"

uitsl::ForEach<
  ThreadCountPayload,
  2
> range{};

// initialize
const uitsl::MpiGuard mpi_guard;
const uitsl::ScopeGuard register_benchmarks( [](){ range.item<1>(); } );

int main(int argc, char** argv) {

  // suppress json output for non-root procs
  int one{1};
  benchmark::Initialize(
    uitsl::is_root() ? &argc : &one,
    argv
  );

  benchmark::RunSpecifiedBenchmarks();

}
