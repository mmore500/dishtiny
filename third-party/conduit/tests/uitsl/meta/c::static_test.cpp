#include <string>
#include <vector>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/types/string.hpp"
#include "cereal/include/cereal/types/vector.hpp"

#include "uitsl/meta/c::static_test.hpp"

TEST_CASE("Test fundamental types", "[nproc:1]") {

  REQUIRE( uitsl::c::static_test<double>() );

  REQUIRE( uitsl::c::static_test<int>() );

}

TEST_CASE("Test stl types", "[nproc:1]") {

  REQUIRE( uitsl::c::static_test<std::string>() );

  REQUIRE( uitsl::c::static_test<int>() );

}

struct foobar { int x; };

TEST_CASE("Test non-serializable types", "[nproc:1]") {

  REQUIRE( !uitsl::c::static_test<foobar>() );

}
