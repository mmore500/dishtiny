#include <limits>
#include <unordered_set>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/math/math_utils.hpp"

TEST_CASE("Test sum", "[nproc:1]") {

  REQUIRE( uitsl::sum(1, 2, 3, 0) == 6 );

  REQUIRE( uitsl::sum(1.0, 2.0, 3.0, 0.5) == 6.5 );

}


TEST_CASE("Test mod", "[nproc:1]") {

  REQUIRE( uitsl::mod(-11, 10) == 9 );
  REQUIRE( uitsl::mod(-10, 10) == 0 );
  REQUIRE( uitsl::mod(-9, 10) == 1 );

  REQUIRE( uitsl::mod(-5, 10) == 5 );
  REQUIRE( uitsl::mod(-4, 10) == 6 );
  REQUIRE( uitsl::mod(-3, 10) == 7 );
  REQUIRE( uitsl::mod(-2, 10) == 8 );
  REQUIRE( uitsl::mod(-1, 10) == 9 );
  REQUIRE( uitsl::mod(0, 10) == 0 );
  REQUIRE( uitsl::mod(1, 10) == 1 );
  REQUIRE( uitsl::mod(2, 10) == 2 );
  REQUIRE( uitsl::mod(3, 10) == 3 );
  REQUIRE( uitsl::mod(4, 10) == 4 );

  REQUIRE( uitsl::mod(9, 10) == 9 );
  REQUIRE( uitsl::mod(10, 10) == 0 );
  REQUIRE( uitsl::mod(11, 10) == 1 );

}

TEST_CASE("Test uitsl::circular_index", "[nproc:1]") {

  REQUIRE( uitsl::circular_index(0, 10, 0) == 0 );

  REQUIRE( uitsl::circular_index(0, 10, 1) == 1 );

  REQUIRE( uitsl::circular_index(0, 10, -1) == 9 );

  REQUIRE( uitsl::circular_index(0, 10, 2) == 2 );

  REQUIRE( uitsl::circular_index(2, 10, 8) == 0 );

  REQUIRE( uitsl::circular_index(3, 1, 0) == 0 );

  REQUIRE( uitsl::circular_index(3, 1, 1) == 0 );

  REQUIRE( uitsl::circular_index(3, 1, -1) == 0 );

  REQUIRE( uitsl::circular_index(3, 2, 0) == 1 );

  REQUIRE( uitsl::circular_index(3, 1, 1) == 0 );

  REQUIRE( uitsl::circular_index(3, 1, -1) == 0 );

}

TEST_CASE("Test stoszt", "[nproc:1]") {

  REQUIRE( uitsl::stoszt("0") == 0 );

  REQUIRE( uitsl::stoszt("1") == 1 );

  REQUIRE( uitsl::stoszt("2") == 2 );

  REQUIRE( uitsl::stoszt("42") == 42 );

  REQUIRE( uitsl::stoszt("4294967296") == 4294967296 );

}

TEST_CASE("Test is_power_of_two", "[nproc:1]") {

  // int
  REQUIRE( !uitsl::is_power_of_two(-65538) );
  REQUIRE( !uitsl::is_power_of_two(-65537) );
  REQUIRE( !uitsl::is_power_of_two(-65536) );
  REQUIRE( !uitsl::is_power_of_two(-65535) );
  REQUIRE( !uitsl::is_power_of_two(-65534) );

  REQUIRE( !uitsl::is_power_of_two(-8) );
  REQUIRE( !uitsl::is_power_of_two(-7) );
  REQUIRE( !uitsl::is_power_of_two(-6) );
  REQUIRE( !uitsl::is_power_of_two(-5) );
  REQUIRE( !uitsl::is_power_of_two(-4) );
  REQUIRE( !uitsl::is_power_of_two(-3) );
  REQUIRE( !uitsl::is_power_of_two(-2) );
  REQUIRE( !uitsl::is_power_of_two(-1) );
  REQUIRE( !uitsl::is_power_of_two(0) );
  REQUIRE( uitsl::is_power_of_two(1) );
  REQUIRE( uitsl::is_power_of_two(2) );
  REQUIRE( !uitsl::is_power_of_two(3) );
  REQUIRE( uitsl::is_power_of_two(4) );
  REQUIRE( !uitsl::is_power_of_two(5) );
  REQUIRE( !uitsl::is_power_of_two(6) );
  REQUIRE( !uitsl::is_power_of_two(7) );
  REQUIRE( uitsl::is_power_of_two(8) );

  REQUIRE( !uitsl::is_power_of_two(65534) );
  REQUIRE( !uitsl::is_power_of_two(65535) );
  REQUIRE( uitsl::is_power_of_two(65536) );
  REQUIRE( !uitsl::is_power_of_two(65537) );
  REQUIRE( !uitsl::is_power_of_two(65538) );

  // size_t
  REQUIRE( !uitsl::is_power_of_two( (size_t) 0) );
  REQUIRE( uitsl::is_power_of_two( (size_t) 1) );
  REQUIRE( uitsl::is_power_of_two( (size_t) 2) );
  REQUIRE( !uitsl::is_power_of_two( (size_t) 3) );
  REQUIRE( uitsl::is_power_of_two( (size_t) 4) );
  REQUIRE( !uitsl::is_power_of_two( (size_t) 5) );
  REQUIRE( !uitsl::is_power_of_two( (size_t) 6) );
  REQUIRE( !uitsl::is_power_of_two( (size_t) 7) );
  REQUIRE( uitsl::is_power_of_two( (size_t) 8) );

  REQUIRE( !uitsl::is_power_of_two( (size_t) 65534) );
  REQUIRE( !uitsl::is_power_of_two( (size_t) 65535) );
  REQUIRE( uitsl::is_power_of_two( (size_t) 65536) );
  REQUIRE( !uitsl::is_power_of_two( (size_t) 65537) );
  REQUIRE( !uitsl::is_power_of_two( (size_t) 65538) );

  REQUIRE( !uitsl::is_power_of_two( (size_t) std::numeric_limits<size_t>::max()) );

  // char
  REQUIRE( !uitsl::is_power_of_two( (char) -8) );
  REQUIRE( !uitsl::is_power_of_two( (char) -7) );
  REQUIRE( !uitsl::is_power_of_two( (char) -6) );
  REQUIRE( !uitsl::is_power_of_two( (char) -5) );
  REQUIRE( !uitsl::is_power_of_two( (char) -4) );
  REQUIRE( !uitsl::is_power_of_two( (char) -3) );
  REQUIRE( !uitsl::is_power_of_two( (char) -2) );
  REQUIRE( !uitsl::is_power_of_two( (char) -1) );
  REQUIRE( !uitsl::is_power_of_two( (char) 0) );
  REQUIRE( uitsl::is_power_of_two( (char) 1) );
  REQUIRE( uitsl::is_power_of_two( (char) 2) );
  REQUIRE( !uitsl::is_power_of_two( (char) 3) );
  REQUIRE( uitsl::is_power_of_two( (char) 4) );
  REQUIRE( !uitsl::is_power_of_two( (char) 5) );
  REQUIRE( !uitsl::is_power_of_two( (char) 6) );
  REQUIRE( !uitsl::is_power_of_two( (char) 7) );
  REQUIRE( uitsl::is_power_of_two( (char) 8) );

}

TEST_CASE("Test num_bits", "[nproc:1]") {

  // int
  REQUIRE( uitsl::num_bits(-65538) == 17 );
  REQUIRE( uitsl::num_bits(-65537) == 17 );
  REQUIRE( uitsl::num_bits(-65536) == 17 );
  REQUIRE( uitsl::num_bits(-65535) == 16 );
  REQUIRE( uitsl::num_bits(-65534) == 16 );

  REQUIRE( uitsl::num_bits(-8) == 4 );
  REQUIRE( uitsl::num_bits(-7) == 3 );
  REQUIRE( uitsl::num_bits(-6) == 3 );
  REQUIRE( uitsl::num_bits(-5) == 3 );
  REQUIRE( uitsl::num_bits(-4) == 3 );
  REQUIRE( uitsl::num_bits(-3) == 2 );
  REQUIRE( uitsl::num_bits(-2) == 2 );
  REQUIRE( uitsl::num_bits(-1) == 1 );
  REQUIRE( uitsl::num_bits(0) == 0 );
  REQUIRE( uitsl::num_bits(1) == 1 );
  REQUIRE( uitsl::num_bits(2) == 2 );
  REQUIRE( uitsl::num_bits(3) == 2 );
  REQUIRE( uitsl::num_bits(4) == 3 );
  REQUIRE( uitsl::num_bits(5) == 3 );
  REQUIRE( uitsl::num_bits(6) == 3 );
  REQUIRE( uitsl::num_bits(7) == 3 );
  REQUIRE( uitsl::num_bits(8) == 4 );

  REQUIRE( uitsl::num_bits(65534) == 16 );
  REQUIRE( uitsl::num_bits(65535) == 16 );
  REQUIRE( uitsl::num_bits(65536) == 17 );
  REQUIRE( uitsl::num_bits(65537) == 17 );
  REQUIRE( uitsl::num_bits(65538) == 17 );

  // size_t
  REQUIRE( uitsl::num_bits( (size_t) 0) == 0 );
  REQUIRE( uitsl::num_bits( (size_t) 1) == 1 );
  REQUIRE( uitsl::num_bits( (size_t) 2) == 2 );
  REQUIRE( uitsl::num_bits( (size_t) 3) == 2 );
  REQUIRE( uitsl::num_bits( (size_t) 4) == 3 );
  REQUIRE( uitsl::num_bits( (size_t) 5) == 3 );
  REQUIRE( uitsl::num_bits( (size_t) 6) == 3 );
  REQUIRE( uitsl::num_bits( (size_t) 7) == 3 );
  REQUIRE( uitsl::num_bits( (size_t) 8) == 4 );

  REQUIRE( uitsl::num_bits( (size_t) 65534) == 16 );
  REQUIRE( uitsl::num_bits( (size_t) 65535) == 16 );
  REQUIRE( uitsl::num_bits( (size_t) 65536) == 17 );
  REQUIRE( uitsl::num_bits( (size_t) 65537) == 17 );
  REQUIRE( uitsl::num_bits( (size_t) 65538) == 17 );

  // char
  REQUIRE( uitsl::num_bits( (char) -8) == 4 );
  REQUIRE( uitsl::num_bits( (char) -7) == 3 );
  REQUIRE( uitsl::num_bits( (char) -6) == 3 );
  REQUIRE( uitsl::num_bits( (char) -5) == 3 );
  REQUIRE( uitsl::num_bits( (char) -4) == 3 );
  REQUIRE( uitsl::num_bits( (char) -3) == 2 );
  REQUIRE( uitsl::num_bits( (char) -2) == 2 );
  REQUIRE( uitsl::num_bits( (char) -1) == 1 );
  REQUIRE( uitsl::num_bits( (char) 0) == 0 );
  REQUIRE( uitsl::num_bits( (char) 1) == 1 );
  REQUIRE( uitsl::num_bits( (char) 2) == 2 );
  REQUIRE( uitsl::num_bits( (char) 3) == 2 );
  REQUIRE( uitsl::num_bits( (char) 4) == 3 );
  REQUIRE( uitsl::num_bits( (char) 5) == 3 );
  REQUIRE( uitsl::num_bits( (char) 6) == 3 );
  REQUIRE( uitsl::num_bits( (char) 7) == 3 );
  REQUIRE( uitsl::num_bits( (char) 8) == 4 );

}

TEST_CASE("test_bit", "[nproc:1]") {

  // int
  REQUIRE( uitsl::test_bit(-8, 8) == 0 );
  REQUIRE( uitsl::test_bit(-8, 3) == 1 );
  REQUIRE( uitsl::test_bit(-2, 0) == 0 );
  REQUIRE( uitsl::test_bit(-2, 1) == 1 );
  REQUIRE( uitsl::test_bit(-1, 0) == 1 );
  REQUIRE( uitsl::test_bit(-1, 1) == 0 );
  REQUIRE( uitsl::test_bit(0, 0) == 0 );
  REQUIRE( uitsl::test_bit(0, 1) == 0 );
  REQUIRE( uitsl::test_bit(0, 2) == 0 );
  REQUIRE( uitsl::test_bit(1, 0) == 1 );
  REQUIRE( uitsl::test_bit(1, 2) == 0 );

  REQUIRE( uitsl::test_bit(8, 15) == 0 );
  REQUIRE( uitsl::test_bit(8, 3) == 1 );

  // size_t
  REQUIRE( uitsl::test_bit( (size_t) 0, 0) == 0 );
  REQUIRE( uitsl::test_bit( (size_t) 0, 1) == 0 );
  REQUIRE( uitsl::test_bit( (size_t) 0, 2) == 0 );
  REQUIRE( uitsl::test_bit( (size_t) 1, 0) == 1 );
  REQUIRE( uitsl::test_bit( (size_t) 1, 2) == 0 );
  REQUIRE( uitsl::test_bit( (size_t) 3, 1) == 1 );
  REQUIRE( uitsl::test_bit( (size_t) 3, 0) == 1 );
  REQUIRE( uitsl::test_bit( (size_t) 3, 2) == 0 );
  REQUIRE( uitsl::test_bit( (size_t) 3, 27) == 0 );
  REQUIRE( uitsl::test_bit( (size_t) 8, 15) == 0 );
  REQUIRE( uitsl::test_bit( (size_t) 8, 3) == 1 );

  for (size_t i = 0; i < sizeof(size_t)*8; ++i) {
    REQUIRE( uitsl::test_bit(std::numeric_limits<size_t>::max(), i) == 1);
    REQUIRE( uitsl::test_bit( (size_t) 0, i) == 0);
  }

  // char
  REQUIRE( uitsl::test_bit( (char) -8, 7) == 0 );
  REQUIRE( uitsl::test_bit( (char) -7, 2) == 1 );
  REQUIRE( uitsl::test_bit( (char) -7, 3) == 0 );
  REQUIRE( uitsl::test_bit( (char) -7, 1) == 1 );
  REQUIRE( uitsl::test_bit( (char) -2, 0) == 0 );
  REQUIRE( uitsl::test_bit( (char) -2, 1) == 1 );
  REQUIRE( uitsl::test_bit( (char) -1, 0) == 1 );
  REQUIRE( uitsl::test_bit( (char) -1, 1) == 0 );
  REQUIRE( uitsl::test_bit( (char) 0, 0) == 0 );
  REQUIRE( uitsl::test_bit( (char) 0, 1) == 0 );
  REQUIRE( uitsl::test_bit( (char) 0, 2) == 0 );
  REQUIRE( uitsl::test_bit( (char) 1, 0) == 1 );
  REQUIRE( uitsl::test_bit( (char) 1, 1) == 0 );
  REQUIRE( uitsl::test_bit( (char) 3, 1) == 1 );
  REQUIRE( uitsl::test_bit( (char) 3, 2) == 0 );
  REQUIRE( uitsl::test_bit( (char) 3, 0) == 1 );
  REQUIRE( uitsl::test_bit( (char) 3, 5) == 0 );
  REQUIRE( uitsl::test_bit( (char) 8, 5) == 0 );
  REQUIRE( uitsl::test_bit( (char) 8, 3) == 1 );

}

TEST_CASE("difference", "[nproc:1]") {

  REQUIRE(uitsl::difference(3,8) == 5);
  REQUIRE(uitsl::difference(12,8) == 4);
  REQUIRE(uitsl::difference(1,0) == 1);
  REQUIRE(uitsl::difference(42,442) == 400);

  for (size_t i = 0; i < 10; ++i) {
    for (size_t j = 0; j < 10; ++j) {
      REQUIRE(uitsl::difference(i, j) == uitsl::difference(j, i));
      REQUIRE(uitsl::difference(i, i) == 0);
    }
  }

}

TEST_CASE("sidebyside_hash", "[nproc:1]") {

  std::unordered_set<size_t> results;

  constexpr size_t n = 100;
  for (size_t a = 0; a < n; ++a) {
    for (size_t b = 0; b < n; ++b) {
      results.insert(uitsl::sidebyside_hash(a, b));
    }
  }

  REQUIRE(results.size() == n * n);

}
