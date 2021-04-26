#include <benchmark/benchmark.h>

#include "conduit/include/uitsl/debug/benchmark_utils.hpp"
#include "Empirical/include/emp/tools/string_utils.hpp"

#include "sgpl/utility/ThreadLocalRandom.hpp"

constexpr bool fill_cores = true;

#include "_BenchHolder.hpp"
#include "_Inst_Loop.hpp"
#include "_SignalGP.hpp"

inst_lib_t inst_lib;
event_lib_t event_lib;
program_t program;

struct SetupA {

  SetupA() {

    inst_lib.AddInst("Fork", sgp::inst_impl::Inst_Fork<signalgp_t, inst_t>, "");
    inst_lib.AddInst("Nop", sgp::inst_impl::Inst_Nop<signalgp_t, inst_t>, "");
    inst_lib.AddInst(
      "Terminate", sgp::inst_impl::Inst_Terminate<signalgp_t, inst_t>, ""
    );
    inst_lib.AddInst("Add", sgp::inst_impl::Inst_Add<signalgp_t, inst_t>, "");
    inst_lib.AddInst("Div", sgp::inst_impl::Inst_Div<signalgp_t, inst_t>, "");
    inst_lib.AddInst("Mod", sgp::inst_impl::Inst_Mod<signalgp_t, inst_t>, "");
    inst_lib.AddInst(
      "Mult", sgp::inst_impl::Inst_Mult<signalgp_t, inst_t>, ""
    );
    inst_lib.AddInst(
      "Sub", sgp::inst_impl::Inst_Sub<signalgp_t, inst_t>, ""
    );
    inst_lib.AddInst("TestEqu", sgp::inst_impl::Inst_TestEqu<signalgp_t, inst_t>, "");
    inst_lib.AddInst(
      "TestLess", sgp::inst_impl::Inst_TestLess<signalgp_t, inst_t>, ""
    );
    inst_lib.AddInst(
      "TestNEqu", sgp::inst_impl::Inst_TestNEqu<signalgp_t, inst_t>, ""
    );
    inst_lib.AddInst("Call", sgp::inst_impl::Inst_Fork<signalgp_t, inst_t>, "");
    inst_lib.AddInst(
      "Routine", sgp::inst_impl::Inst_Fork<signalgp_t, inst_t>, ""
    );

    inst_lib.AddInst(
      "If",
      sgp::inst_impl::Inst_Div<signalgp_t, inst_t>,
      "",
      {inst_prop_t::BLOCK_DEF}
    );
    inst_lib.AddInst(
      "While",
      sgp::inst_impl::Inst_Div<signalgp_t, inst_t>,
      "",
      {inst_prop_t::BLOCK_DEF}
    );
    inst_lib.AddInst(
      "Countdown",
      sgp::inst_impl::Inst_Div<signalgp_t, inst_t>,
      "",
      {inst_prop_t::BLOCK_DEF}
    );
    inst_lib.AddInst(
      "Close",
      sgp::inst_impl::Inst_Close<signalgp_t, inst_t>,
      "",
      {inst_prop_t::BLOCK_CLOSE}
    );

    inst_lib.AddInst(
      "AdjRegulator", sgp::inst_impl::Inst_AdjRegulator<signalgp_t, inst_t>, ""
    );
    inst_lib.AddInst(
      "SenseRegulator", sgp::inst_impl::Inst_SenseRegulator<signalgp_t, inst_t>, ""
    );
    inst_lib.AddInst(
      "SetRegulator", sgp::inst_impl::Inst_SetRegulator<signalgp_t, inst_t>, ""
    );
    inst_lib.AddInst("Call", sgp::inst_impl::Inst_Call<signalgp_t, inst_t>, "");
    inst_lib.AddInst("Dec", sgp::inst_impl::Inst_Dec<signalgp_t, inst_t>, "");
    inst_lib.AddInst(
      "Generate",
      [](signalgp_t& hw, const inst_t& inst) {
        auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
        // Increment value in local memory @ [ARG0]
        call_state.GetMemory().AccessWorking(inst.GetArg(0))
          = sgpl::tlrand.Get().GetDouble();
      },
      ""
    );
    inst_lib.AddInst("Inc", sgp::inst_impl::Inst_Inc<signalgp_t, inst_t>, "");
    inst_lib.AddInst(
      "Negate",
      [](signalgp_t& hw, const inst_t& inst) {
        auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
        // Increment value in local memory @ [ARG0]
        call_state.GetMemory().AccessWorking(inst.GetArg(0)) *= -1;
      },
      ""
    );
    inst_lib.AddInst("Not", sgp::inst_impl::Inst_Not<signalgp_t, inst_t>, "");
    inst_lib.AddInst(
      "Terminal", sgp::inst_impl::Inst_Terminal<signalgp_t, inst_t>, ""
    );

  }

} setup_a;


program_t MakeProgram() {

  emp::Random rand;

  return sgp::GenRandLinearFunctionsProgram<signalgp_t, 64>(
    rand,
    inst_lib,
    {1, 1}, // num functions
    1, // num func tags
    {100, 100}, // num instructions
    1, // num inst tags
    3, // num inst args
    {0, 5}
  );

}

BenchHolder<1> bench1{ MakeProgram, inst_lib, event_lib };
BenchHolder<32> bench32{ MakeProgram, inst_lib, event_lib };
BenchHolder<1024> bench1024{ MakeProgram, inst_lib, event_lib };
BenchHolder<32768> bench32768{ MakeProgram, inst_lib, event_lib };

template<size_t NUM_AGENTS>
void Register(BenchHolder<NUM_AGENTS>& holder) {
  auto res = benchmark::RegisterBenchmark(
    emp::to_string(
      "Vanilla_Complete<", NUM_AGENTS, ">"
    ).c_str(),
    [&](benchmark::State& state){ holder.Run(state); }
  );

  uitsl::report_confidence<20>( res );

}

struct SetupB {

  SetupB() {
    Register<1>( bench1 );
    Register<32>( bench32 );
    Register<1024>( bench1024);
    Register<32768>( bench32768 );
  }

} setup_b;

// Run the benchmark
BENCHMARK_MAIN();
