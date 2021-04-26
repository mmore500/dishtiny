#include <map>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/algorithm/upper_uniquify.hpp"

TEST_CASE("Test upper_uniquify", "[nproc:1]")
{

  std::multimap<int, int> target{
    {1, 0}, {1, 1}, {2, 0}, {1, 2}, {3, 0}, {3, 1}
  };

  uitsl::upper_uniquify( target );

  REQUIRE( target.size() == 3 );

  REQUIRE( target.count( 1 ) == 1 );
  REQUIRE( target.find( 1 )->second == 2 );

  REQUIRE( target.count( 2 ) == 1 );
  REQUIRE( target.find( 2 )->second == 0 );

  REQUIRE( target.count( 3 ) == 1 );
  REQUIRE( target.find( 3 )->second == 1 );


}
