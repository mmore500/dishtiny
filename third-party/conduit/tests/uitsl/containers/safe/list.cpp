#include <string>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/containers/safe/list.hpp"

TEST_CASE("test list", "[nproc:1]") {

  // TODO expand stub test
  uitsl::safe::list<std::string> list;

  REQUIRE(list.empty());
  list.push_back("hello");
  REQUIRE(list.front() == "hello");

  REQUIRE(list.back() == "hello");

}
