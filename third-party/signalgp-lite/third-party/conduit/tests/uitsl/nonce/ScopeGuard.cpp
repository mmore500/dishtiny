#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/nonce/ScopeGuard.hpp"

TEST_CASE("Test ScopeGuard with default OnDestruct")
{

  size_t i{};
  REQUIRE( i == 0 );

  {
  const uitsl::ScopeGuard guard{ [&](){++i;} };
  REQUIRE( i == 1 );
  }

  REQUIRE( i == 1 );


}

TEST_CASE("Test ScopeGuard with custom OnDestruct")
{

  size_t i{};
  REQUIRE( i == 0 );

  {
  const uitsl::ScopeGuard guard{ [&](){++i;}, [&](){--i;} };
  REQUIRE( i == 1 );
  }

  REQUIRE( i == 0 );


}
