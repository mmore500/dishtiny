#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/WarnOnce.hpp"

TEST_CASE("WarnOnce", "[nproc:1]") {

  static const uitsl::WarnOnce warning{"howdy"};

}
