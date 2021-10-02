#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "dish2/utility/sha256_reduce.hpp"

TEST_CASE("sha256_reduce determinstic") {

  REQUIRE(
    dish2::sha256_reduce(emp::vector<uint32_t>{ 0, 0, 0 })
    == dish2::sha256_reduce(emp::vector<uint32_t>{ 0, 0, 0 })
  );
  REQUIRE(
    dish2::sha256_reduce(emp::vector<uint32_t>{ 0, 1 })
    == dish2::sha256_reduce(emp::vector<uint32_t>{ 0, 1 })
  );

}

TEST_CASE("sha256_reduce unique") {

  emp::vector<uint32_t> data{
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 1024, 0 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 1048576, 0 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 0, 0 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 1, 0 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 1, 1, 0 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 9, 0 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 0, 1 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 1048576 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 0 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 1 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 1, 1 } ),
    dish2::sha256_reduce( emp::vector<uint32_t>{ 0, 9 } ),
    dish2::sha256_reduce( "" ),
    dish2::sha256_reduce( "asdf" ),
    dish2::sha256_reduce( "foobar" )
  };

  REQUIRE(
    std::set<uint32_t>(std::begin(data), std::end(data)).size() == data.size()
  );

}
