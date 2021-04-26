#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "sgpl/algorithm/sloppy_copy.hpp"
#include "sgpl/utility/ThreadLocalRandom.hpp"

TEST_CASE("Test sloppy_copy without mutation") {

  sgpl::tlrand.Get() = emp::Random(1);

  emp::vector<int> original( 100 );
  std::iota( std::begin( original ), std::end( original ), 0 );

  auto [copy, num_muts] = sgpl::sloppy_copy( original, 1.0f, 0 );

  REQUIRE( original == copy );
  REQUIRE( num_muts == 0 );

}

TEST_CASE("Test sloppy_copy with mild mutation") {

  sgpl::tlrand.Get() = emp::Random(1);

  emp::vector<int> original( 100 );
  std::iota( std::begin( original ), std::end( original ), 0 );

  auto [copy, num_muts] = sgpl::sloppy_copy( original, 0.1f, 2 );

  REQUIRE( copy != original );
  REQUIRE( num_muts > 0 );

  // do all of the items in copy come from somewhere in the original?
  REQUIRE( std::all_of(
    std::begin( copy ),
    std::begin( copy ),
    [&original](const int val){
      return std::find(
        std::begin( original ),
        std::end( original ),
        val
      ) != std::end( original );
    }
  ) );

}

TEST_CASE("Test sloppy_copy with severe mutation") {

  sgpl::tlrand.Get() = emp::Random(1);

  emp::vector<int> original( 100 );
  std::iota( std::begin( original ), std::end( original ), 0 );

  auto [copy, num_muts] = sgpl::sloppy_copy( original, 0.1f, original.size() );

  REQUIRE( copy != original );
  REQUIRE( num_muts > 0 );

  // do all of the items in copy come from somewhere in the original?
  REQUIRE( std::all_of(
    std::begin( copy ),
    std::begin( copy ),
    [&original](const int val){
      return std::find(
        std::begin( original ),
        std::end( original ),
        val
      ) != std::end( original );
    }
  ) );

}
