#include <atomic>

#include <benchmark/benchmark.h>

#include "conduit/include/uitsl/debug/benchmark_utils.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"
#include "conduit/include/uitsl/polyfill/barrier.hpp"
#include "Empirical/include/emp/tools/string_utils.hpp"

#include "dish2/config/cfg.hpp"
#include "dish2/debug/backtrace_enable.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const uitsl::MpiGuard guard;

template<size_t NUM_CELLS>
static void DoBench(benchmark::State& state) {

  // Perform setup here
  size_t cell_update_counter{};

  // benchmark goes here
  for (auto _ : state) ++cell_update_counter;

  // these are summed over threads
  state.counters["Num Cells"] = NUM_CELLS / state.threads;
  state.counters["Num Threads"] = 1;
  state.counters["Cell-Updates Executed"] = cell_update_counter;

}

template<size_t NUM_CELLS, size_t NUM_THREADS=1>
struct Setup {

  Setup() {

    auto res = benchmark::RegisterBenchmark(
      emp::to_string(
        "ThreadWorld", "<", NUM_CELLS, ">"
      ).c_str(),
      DoBench<NUM_CELLS>
    );

    res->Threads( NUM_THREADS );

    uitsl::report_confidence<10>( res );

  }

};

Setup<1> a;
Setup<32> b;
Setup<1024> c;
Setup<32768> d;

Setup<32, 2> e;
Setup<1024, 2> f;
Setup<32768, 2> g;

Setup<32, 4> h;
Setup<1024, 4> i;
Setup<32768, 4> j;

// Run the benchmark
BENCHMARK_MAIN();
