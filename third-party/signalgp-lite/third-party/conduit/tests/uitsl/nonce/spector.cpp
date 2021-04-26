#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/nonce/spector.hpp"

TEST_CASE("Test spector as vector") {

  const uitsl::spector<int> spector1;
  REQUIRE( spector1.size() == 0 );
  REQUIRE( spector1.size_bytes() == 0 );

  uitsl::spector<char> spector2( 100 );
  REQUIRE( spector2.size() == 100 );
  REQUIRE( spector2.size_bytes() == 100 );

  spector2.data()[0] = 'a';
  spector2.data()[2] = 'c';
  REQUIRE( spector2.data()[0] == 'a' );
  REQUIRE( spector2.data()[2] == 'c' );

  uitsl::spector<int> spector3( std::in_place_type_t<emp::vector<int>>{}, 100 );
  REQUIRE( spector3.size() == 100 );
  spector3.resize(50);
  REQUIRE( spector3.size() == 50 );
  REQUIRE( spector3.size_bytes() == 50 * sizeof(int));

}

TEST_CASE("Test spector as span") {

  emp::vector<size_t> vec{1, 2, 3};

  const uitsl::spector<int> spector1{
    std::in_place_type_t<std::span<int>>{}
  };
  REQUIRE( spector1.size() == 0 );
  REQUIRE( spector1.size_bytes() == 0 );

  uitsl::spector<size_t> spector2(
    std::in_place_type_t<std::span<size_t>>{},
    vec.data(),
    vec.size()
  );

  REQUIRE( spector2.size() == 3 );
  REQUIRE( spector2.size_bytes() == 3 * sizeof(size_t) );

  REQUIRE( spector2.data()[0] == 1);
  REQUIRE( spector2.data()[1] == 2);
  REQUIRE( spector2.data()[2] == 3);

  uitsl::spector<int> spector3{100};
  REQUIRE( spector3.size() == 100 );
  REQUIRE( spector3.size_bytes() == 100 * sizeof(int));

  spector3.data()[3] = 4;
  REQUIRE( spector3.data()[3] == 4);

}
