#include "uitsl/meta/ForEach.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/nonce/ScopeGuard.hpp"

#include "../DuctBenchmarkRegistration.hpp"

// sample doubling thread counts
uitsl::ForEach<
  ThreadCountPayload,
  128,
  2,
  std::multiplies<size_t>
> range{};

const uitsl::MpiGuard mguard;

const uitsl::ScopeGuard sguard{ [](){ range.item<1>(); } };

BENCHMARK_MAIN();
