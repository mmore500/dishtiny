#include <set>
#include <string>
#include <vector>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/types/string.hpp"
#include "cereal/include/cereal/types/vector.hpp"

#include "uitsl/meta/s::static_test.hpp"

TEST_CASE("Testr s::static_test", "[nproc:1]") {
  SECTION("Test valid types") {

    REQUIRE( uitsl::s::static_test<std::vector<int>>() );

    REQUIRE( uitsl::s::static_test<std::string>() );

  }

  SECTION("Test invalid types") {

    REQUIRE( !uitsl::s::static_test<int>() );

    REQUIRE( !uitsl::s::static_test<double>() );

    REQUIRE( !uitsl::s::static_test<std::set<double>>() );

  }
}
