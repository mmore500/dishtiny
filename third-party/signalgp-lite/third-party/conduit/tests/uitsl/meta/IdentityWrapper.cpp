#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/meta/IdentityWrapper.hpp"

TEST_CASE("IdentityWrapper", "[nproc:1]") {

  uitsl::IdentityWrapper<std::string> wrapped{"present"};
  REQUIRE(wrapped == "present");
  REQUIRE(wrapped.size() == 7);

}
