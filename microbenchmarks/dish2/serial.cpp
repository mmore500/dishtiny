#include <benchmark/benchmark.h>

#include "conduit/include/uitsl/debug/benchmark_utils.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"
#include "Empirical/source/tools/string_utils.h"

#include "dish2/config/cfg.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const uitsl::MpiGuard guard;

template<size_t NUM_CELLS>
static void DoBench(benchmark::State& state) {

  // Perform setup here
  dish2::cfg.Set("N_CELLS", emp::to_string(NUM_CELLS));
  auto tw = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);

  // benchmark goes here
  for (auto _ : state) tw.Update();

  state.counters["num cells"] = NUM_CELLS;
  state.counters["num threads"] = dish2::cfg.N_THREADS();
  state.counters["Core-Nanoseconds per Cell Update"] = benchmark::Counter(
    static_cast<double>( NUM_CELLS ) * tw.GetUpdate() / std::nano::den,
    benchmark::Counter::kAvgThreadsRate | benchmark::Counter::kInvert
  );

}

template<size_t NUM_CELLS>
struct Setup {

  Setup() {

    auto res = benchmark::RegisterBenchmark(
      emp::to_string(
        "ThreadWorld", "<", NUM_CELLS, ">"
      ).c_str(),
      DoBench<NUM_CELLS>
    );

    uitsl::report_confidence<10>( res );

  }

};

Setup<1> s1;
Setup<32> s32;
Setup<1024> s1024;
Setup<32768> s32768;

// Run the benchmark
BENCHMARK_MAIN();
