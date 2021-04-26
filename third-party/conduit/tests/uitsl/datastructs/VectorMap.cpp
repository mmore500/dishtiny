#include <string>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/datastructs/VectorMap.hpp"

TEST_CASE("VectorMap", "[nproc:1]") {

  uitsl::VectorMap<size_t, std::string> map;

  REQUIRE( map[2] == "");
  REQUIRE( map.at(2) == "");

  map[1] = "howdy";
  REQUIRE( map[1] == "howdy" );

  map[4] = "cowsay";
  REQUIRE( map.at(4) == "cowsay" );
  REQUIRE( map[4] == "cowsay" );

  REQUIRE( map.at(1) == "howdy" );
  REQUIRE( map[1] == "howdy" );

  map[0] = "apple";
  map[3] = "orange";

  std::string res;

  for (auto& [__, val] : map) res.append(val);

  for (const auto& [__, val] : map) res.append(val);

  REQUIRE( res == "applehowdyorangecowsayapplehowdyorangecowsay" );

}
