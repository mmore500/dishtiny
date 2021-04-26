#include <string>
#include <tuple>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/meta/TupleConstructEach.hpp"

TEST_CASE("Test TupleConstructEach same", "[nproc:1]") {

  using type = std::tuple<int, int, int>;

  REQUIRE(
    uitsl::TupleConstructEach<type>::Make(42)
    == type{ 42, 42, 42 }
  );

}

TEST_CASE("Test TupleConstructEach different", "[nproc:1]") {

  using type = std::tuple<int, double, char, int>;

  REQUIRE(
    uitsl::TupleConstructEach<type>::Make(42)
    == type{ 42, 42, 42, 42 }
  );

}

TEST_CASE("Test TupleConstructEach multi", "[nproc:1]") {

  using type = std::tuple<std::vector<char>, std::string>;

  REQUIRE(
    uitsl::TupleConstructEach<type>::Make(3, 'a')
    == type{ {'a', 'a', 'a'}, "aaa" }
  );

}
