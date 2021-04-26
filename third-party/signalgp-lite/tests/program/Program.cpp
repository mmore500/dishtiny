#include <fstream>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "cereal/include/cereal/archives/binary.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "Empirical/include/emp/math/Random.hpp"

#include "sgpl/algorithm/execute_cpu.hpp"
#include "sgpl/hardware/Cpu.hpp"
#include "sgpl/program/Program.hpp"
#include "sgpl/spec/Spec.hpp"

using spec_t = sgpl::Spec<>;

template<typename InArchive, typename OutArchive>
void DoSerializationTest(const std::string& filename) {

  sgpl::Program<spec_t> original{ 100 };

  std::ofstream os(filename);

  { OutArchive archive( os ); archive( original ); }

  os.close();

  std::ifstream is(filename);

  sgpl::Program<spec_t> reconstituted;

  { InArchive archive( is ); archive( reconstituted ); }

  REQUIRE( reconstituted == original );

  os.close();
}

TEST_CASE("Test Program Binary Serialization") {

  DoSerializationTest<cereal::BinaryInputArchive, cereal::BinaryOutputArchive>(
    "program.bin"
  );

}


TEST_CASE("Test Program Text Serialization") {

  DoSerializationTest<cereal::JSONInputArchive, cereal::JSONOutputArchive>(
    "program.json"
  );

}

TEST_CASE("Program ApplyPointMutations") {

  const sgpl::Program<spec_t> original{ 100 };

  auto copy = original;

  REQUIRE( copy == original );

  REQUIRE( 0 == copy.ApplyPointMutations( 0.0f ) );

  REQUIRE( copy == original );

  REQUIRE( 0 < copy.ApplyPointMutations( 0.05f ) );

  REQUIRE( copy != original );

  // did rectification work?
  sgpl::Cpu<spec_t> cpu;
  cpu.InitializeAnchors( copy );

  REQUIRE( cpu.TryLaunchCore() );
  REQUIRE( cpu.TryLaunchCore() );
  REQUIRE( cpu.TryLaunchCore() );

  // TODO flesh out stub test
  sgpl::execute_cpu<spec_t>( std::mega::num, cpu, copy );

}
