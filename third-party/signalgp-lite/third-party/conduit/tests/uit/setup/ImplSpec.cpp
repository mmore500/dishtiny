#include "Catch/single_include/catch2/catch.hpp"

#include "uit/setup/ImplSpec.hpp"


TEST_CASE("Test ImplSelect") {

  uit::ImplSelect<>{};

}

TEST_CASE("Test ImplSpec") {

  uit::ImplSpec<char>{};

}
