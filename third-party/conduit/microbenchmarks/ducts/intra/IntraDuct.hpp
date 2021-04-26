#include "uitsl/meta/ForEach.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/nonce/ScopeGuard.hpp"

#include "../DuctBenchmarkRegistration.hpp"

uitsl::ForEach<
  ThreadCountPayload,
  2
> range{};

const uitsl::MpiGuard mguard{};

const uitsl::ScopeGuard sguard{ [](){ range.item<1>(); } };

BENCHMARK_MAIN();
