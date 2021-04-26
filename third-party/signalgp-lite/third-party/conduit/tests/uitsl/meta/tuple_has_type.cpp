#include <string>
#include <tuple>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/meta/tuple_has_type.hpp"

TEST_CASE("Test tuple_has_type true", "[nproc:1]") {

  using type = std::tuple<int, double, std::string, int>;

  REQUIRE( uitsl::tuple_has_type<int, type>::value );

  REQUIRE( uitsl::tuple_has_type<double, type>::value );

  REQUIRE( uitsl::tuple_has_type<std::string, type>::value );

}

TEST_CASE("Test tuple_has_type false", "[nproc:1]") {

  using type = std::tuple<int, double, std::string, int>;

  REQUIRE( uitsl::tuple_has_type<char, type>::value == false );

  REQUIRE( uitsl::tuple_has_type<std::true_type, type>::value == false );

  REQUIRE( uitsl::tuple_has_type<short, type>::value == false );


}
