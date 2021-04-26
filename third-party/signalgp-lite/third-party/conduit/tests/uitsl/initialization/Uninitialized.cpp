#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/initialization/Uninitialized.hpp"

TEST_CASE("Test Uninitialized", "[nproc:1]") {

  uitsl::Uninitialized<int> val1;
  val1 = 10;
  REQUIRE( val1 == 10 );
  REQUIRE( *(&val1) == 10 );

  uitsl::Uninitialized<double> val2;
  val2 = -10.0;
  REQUIRE( val2 == -10.0 );
  REQUIRE( *(&val2) == -10.0 );

}
