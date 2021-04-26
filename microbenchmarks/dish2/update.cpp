#include <atomic>

#include <benchmark/benchmark.h>

#include "conduit/include/uitsl/debug/benchmark_utils.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"
#include "conduit/include/uitsl/polyfill/barrier.hpp"
#include "Empirical/include/emp/base/optional.hpp"
#include "Empirical/include/emp/tools/string_utils.hpp"

#include "dish2/config/cfg.hpp"
#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/debug/backtrace_enable.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const uitsl::MpiGuard guard;

emp::optional<dish2::ProcWorld<dish2::Spec_default>> proc_world;
std::atomic<size_t> flag{false};

template<size_t NUM_CELLS>
static void DoBench(benchmark::State& state) {

  const dish2::TemporaryThreadIdxOverride override( state.thread_index );

  if (state.thread_index == 0) {
    dish2::cfg.Set("N_CELLS", emp::to_string(NUM_CELLS));
    dish2::cfg.Set("N_THREADS", emp::to_string(state.threads));
    // disable some services to prevent extinction
    dish2::cfg.Set("APOPTOSIS_SERVICE_FREQUENCY", "0");
    dish2::cfg.Set("CELL_AGE_SERVICE_FREQUENCY", "0");
    dish2::cfg.Set("QUORUM_CAP_SERVICE_FREQUENCY", "0");
    netuit::internal::MeshIDCounter::Reset();
    proc_world.emplace();
    flag = true;
  } else while (flag == false);

  // Perform setup here
  auto tw = proc_world->MakeThreadWorld();

  // benchmark goes here
  for (auto _ : state) tw.Update();

  flag = false;

  // these are summed over threads
  state.counters["Num Cells"] = tw.GetSize();
  state.counters["Num Threads"] = 1;
  state.counters["Cell-Updates Executed"] = tw.GetUpdate() * tw.GetSize();

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
