#include "conduit/include/uitsl/countdown/ProgressBar.hpp"
#include "conduit/include/uitsl/countdown/Timer.hpp"
#include "conduit/include/uitsl/polyfill/identity.hpp"
#include "Empirical/include/emp/math/Random.hpp"

#include "sgpl/algorithm/execute_cpu.hpp"
#include "sgpl/hardware/Cpu.hpp"
#include "sgpl/program/Program.hpp"
#include "sgpl/spec/Spec.hpp"

using spec_t = sgpl::Spec<>;

using timekeeper_t = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;

using bar_t = uitsl::ProgressBar<timekeeper_t>;

emp::Random rng;

void fuzz_one() {

  sgpl::Cpu<spec_t> cpu;
  sgpl::Program<spec_t> program{ 100 };
  cpu.InitializeAnchors( program );

  for ( [[maybe_unused]] auto j : timekeeper_t{ std::chrono::seconds{ 1 } }) {
    while ( cpu.TryLaunchCore( emp::BitSet<64>(rng) ) ) ;
    sgpl::execute_cpu<spec_t>( std::kilo::num, cpu, program );
  }

}

int main() {

  for ([[maybe_unused]] auto i : bar_t{ std::chrono::minutes{ 10 } })
    fuzz_one();

}
