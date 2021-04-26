#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/OncePerThreadChecker.hpp"

TEST_CASE("OncePerThreadChecker", "[nproc:1]") {

  // TODO expand stub test
  uitsl::OncePerThreadChecker checker;
  checker.Check();

}
