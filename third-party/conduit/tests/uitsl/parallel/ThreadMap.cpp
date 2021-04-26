#include <stddef.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/parallel/ThreadMap.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"

TEST_CASE("ThreadMap") {

  // TODO expand stub test
  uitsl::ThreadMap<size_t> map;

  uitsl::ThreadTeam team;

  for (size_t thread = 0; thread < 4; ++thread) {
    team.Add([&map](){
      REQUIRE(++map.GetWithDefault() == 1);
      REQUIRE(++map.GetWithDefault() == 2);
    });
  }

  team.Join();

  REQUIRE(++map.GetWithDefault() == 1);
  REQUIRE(++map.GetWithDefault() == 2);
  REQUIRE(map.GetSize() == 5);

}
