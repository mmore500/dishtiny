#include <limits>
#include <unordered_set>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/math/shift_mod.hpp"

TEST_CASE("Test shift_mod", "[nproc:1]") {

  REQUIRE( uitsl::shift_mod(0, 1) == 0 );
  REQUIRE( uitsl::shift_mod(1, 1) == 0 );
  REQUIRE( uitsl::shift_mod(2, 1) == 0 );
  REQUIRE( uitsl::shift_mod(3, 1) == 0 );
  REQUIRE( uitsl::shift_mod(4, 1) == 0 );
  REQUIRE( uitsl::shift_mod(5, 1) == 0 );
  REQUIRE( uitsl::shift_mod(6, 1) == 0 );
  REQUIRE( uitsl::shift_mod(7, 1) == 0 );
  REQUIRE( uitsl::shift_mod(8, 1) == 0 );

  REQUIRE( uitsl::shift_mod(0, 2) == 0 );
  REQUIRE( uitsl::shift_mod(1, 2) == 1 );
  REQUIRE( uitsl::shift_mod(2, 2) == 0 );
  REQUIRE( uitsl::shift_mod(3, 2) == 1 );
  REQUIRE( uitsl::shift_mod(4, 2) == 0 );
  REQUIRE( uitsl::shift_mod(5, 2) == 1 );
  REQUIRE( uitsl::shift_mod(6, 2) == 0 );
  REQUIRE( uitsl::shift_mod(7, 2) == 1 );
  REQUIRE( uitsl::shift_mod(8, 2) == 0 );

  REQUIRE( uitsl::shift_mod(0, 4) == 0 );
  REQUIRE( uitsl::shift_mod(1, 4) == 1 );
  REQUIRE( uitsl::shift_mod(2, 4) == 2 );
  REQUIRE( uitsl::shift_mod(3, 4) == 3 );
  REQUIRE( uitsl::shift_mod(4, 4) == 0 );
  REQUIRE( uitsl::shift_mod(5, 4) == 1 );
  REQUIRE( uitsl::shift_mod(6, 4) == 2 );
  REQUIRE( uitsl::shift_mod(7, 4) == 3 );
  REQUIRE( uitsl::shift_mod(8, 4) == 0 );

  REQUIRE( uitsl::shift_mod(0, 8) == 0 );
  REQUIRE( uitsl::shift_mod(1, 8) == 1 );
  REQUIRE( uitsl::shift_mod(2, 8) == 2 );
  REQUIRE( uitsl::shift_mod(3, 8) == 3 );
  REQUIRE( uitsl::shift_mod(4, 8) == 4 );
  REQUIRE( uitsl::shift_mod(5, 8) == 5 );
  REQUIRE( uitsl::shift_mod(6, 8) == 6 );
  REQUIRE( uitsl::shift_mod(7, 8) == 7 );
  REQUIRE( uitsl::shift_mod(8, 8) == 0 );

  REQUIRE( uitsl::shift_mod(0, 16) == 0 );
  REQUIRE( uitsl::shift_mod(1, 16) == 1 );
  REQUIRE( uitsl::shift_mod(2, 16) == 2 );
  REQUIRE( uitsl::shift_mod(3, 16) == 3 );
  REQUIRE( uitsl::shift_mod(4, 16) == 4 );
  REQUIRE( uitsl::shift_mod(5, 16) == 5 );
  REQUIRE( uitsl::shift_mod(6, 16) == 6 );
  REQUIRE( uitsl::shift_mod(7, 16) == 7 );
  REQUIRE( uitsl::shift_mod(8, 16) == 8 );

}
