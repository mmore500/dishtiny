#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/nonce/CircularIndex.hpp"

TEST_CASE("Test CircularIndex")
{
  uitsl::CircularIndex<10> i;

  REQUIRE( i == 0 );

  REQUIRE ( ++i == 1 );

  REQUIRE ( i++ == 1 );

  REQUIRE ( i == 2 );

  REQUIRE ( (i += 8) == 0 );

  uitsl::CircularIndex<10> j(i);

  REQUIRE( j == 0 );

  REQUIRE ( --j == 9 );

  REQUIRE ( (j+1) == 0 );

  REQUIRE ( (j+1) == 0 );

  REQUIRE ( (j-1) == 8 );

  REQUIRE ( (j-1) == 8 );

  REQUIRE(uitsl::CircularIndex<10>(-1) == 9);

  REQUIRE(uitsl::CircularIndex<10>{} - 1 == 9);

  REQUIRE(uitsl::CircularIndex<10>{} - 2 == 8);

  REQUIRE(uitsl::CircularIndex<10>{} + -1 == 9);

  REQUIRE(uitsl::CircularIndex<10>{} + -2 == 8);

}
