#include <string>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/containers/safe/deque.hpp"

TEST_CASE("deque", "[nproc:1]") {

  // TODO expand stub test
  uitsl::safe::deque<std::string> deque;

  REQUIRE(deque.empty());
  deque.push_back("hello");
  REQUIRE(deque[0] == "hello");

  REQUIRE(deque.back() == "hello");

}
