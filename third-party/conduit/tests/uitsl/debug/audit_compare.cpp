#include <limits>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/audit_compare.hpp"


TEST_CASE("Test audit_less<signed, signed>", "[nproc:1]") {

  // shorts
  for (short i = -100; i < 100; ++i) {
    REQUIRE( uitsl::audit_less(i, i+2) );
    REQUIRE( uitsl::audit_less(i, i+1) );
    REQUIRE( !uitsl::audit_less(i, i) );
    REQUIRE( !uitsl::audit_less(i+1, i) );
    REQUIRE( !uitsl::audit_less(i+2, i) );
    REQUIRE( uitsl::audit_less(i, (int) 100) );
    REQUIRE( uitsl::audit_less( (long) -101, i) );
  }

  // ints
  for (int i = -100; i < 100; ++i) {
    REQUIRE( uitsl::audit_less(i, i+2) );
    REQUIRE( uitsl::audit_less(i, i+1) );
    REQUIRE( !uitsl::audit_less(i, i) );
    REQUIRE( !uitsl::audit_less(i+1, i) );
    REQUIRE( !uitsl::audit_less(i+2, i) );
    REQUIRE( uitsl::audit_less(i, (short) 100) );
    REQUIRE( uitsl::audit_less( (short) -101, i) );
  }

}

TEST_CASE("Test audit_less<unsigned, unsigned>", "[nproc:1]") {

  // big size_t
  for (
    size_t i = std::numeric_limits<size_t>::max() - 99;
    i < std::numeric_limits<size_t>::max();
    ++i
  ) {
    REQUIRE( uitsl::audit_less(i, i+1) );
    REQUIRE( !uitsl::audit_less(i, i) );
    REQUIRE( !uitsl::audit_less(i+1, i) );
    REQUIRE( uitsl::audit_less((unsigned char) 0, i) );
  }

  // size_t wraparound
  REQUIRE( uitsl::audit_less( (size_t) 0,  (size_t) -1) );
  REQUIRE( uitsl::audit_less( (size_t) 0,  (size_t) 1) );
  REQUIRE( !uitsl::audit_less( (size_t) -1,  (size_t) 0) );
  REQUIRE( !uitsl::audit_less( (size_t) 1,  (size_t) 0) );
  REQUIRE( !uitsl::audit_less( (size_t) -1,  (size_t) -1) );
  REQUIRE( !uitsl::audit_less( (size_t) 0,  (size_t) 0) );

  // small size_t
  for (size_t i = 0; i < 99; ++i) {
    REQUIRE( uitsl::audit_less(i, i+1) );
    REQUIRE( !uitsl::audit_less(i, i) );
    REQUIRE( !uitsl::audit_less(i+1, i) );
    REQUIRE( uitsl::audit_less(i, (unsigned char) 100) );
  }

  // unsigned char
  for (unsigned char i = 0; i < 99; ++i) {
    REQUIRE( uitsl::audit_less(i, i+1) );
    REQUIRE( !uitsl::audit_less(i, i) );
    REQUIRE( !uitsl::audit_less(i+1, i) );
    REQUIRE( uitsl::audit_less(i, (size_t) 100) );
  }

}

TEST_CASE("Test audit_less<signed, unsigned>", "[nproc:1]") {

  REQUIRE( uitsl::audit_less( (short) -1, (size_t) 0) );
  REQUIRE( uitsl::audit_less( (int) -1, (size_t) 0) );
  REQUIRE( uitsl::audit_less( (int) 0, (size_t) 1) );

  #ifndef NDEBUG
  REQUIRE_THROWS( uitsl::audit_less( (int) 0, (size_t) -1) );
  #endif

  REQUIRE( !uitsl::audit_less( (int) 1, (unsigned short) 0) );
  REQUIRE( !uitsl::audit_less( (int) 2, (size_t) 1) );
  REQUIRE( !uitsl::audit_less( (short) 42, (size_t) 25) );


  #ifndef NDEBUG
  REQUIRE_THROWS( uitsl::audit_less( (int) -1, (unsigned short) -1) );
  #endif
  REQUIRE( !uitsl::audit_less( (short) 0, (size_t) 0) );
  REQUIRE( !uitsl::audit_less( (int) 1, (size_t) 1) );

}

TEST_CASE("Test audit_less<unsigned, signed>", "[nproc:1]") {

  REQUIRE( !uitsl::audit_less( (size_t) 0, (short) -1) );
  REQUIRE( !uitsl::audit_less( (size_t) 0, (int) -1) );
  REQUIRE( !uitsl::audit_less( (size_t) 1, (int) 0) );

  #ifndef NDEBUG
  REQUIRE_THROWS( !uitsl::audit_less( (size_t) -1, (int) 0) );
  #endif
  REQUIRE( uitsl::audit_less( (unsigned short) 0, (int) 1) );
  REQUIRE( uitsl::audit_less( (size_t) 1, (int) 2) );
  REQUIRE( uitsl::audit_less( (size_t) 25, (short) 42) );

  #ifndef NDEBUG
  REQUIRE_THROWS( !uitsl::audit_less( (unsigned short) -1, (int) -1) );
  #endif
  REQUIRE( !uitsl::audit_less( (size_t) 0, (short) 0) );
  REQUIRE( !uitsl::audit_less( (size_t) 1, (int) 1) );

}

TEST_CASE("Test audit_greater<signed, signed>", "[nproc:1]") {

  // shorts
  for (short i = -100; i < 100; ++i) {
    REQUIRE( !uitsl::audit_greater(i, i+2) );
    REQUIRE( !uitsl::audit_greater(i, i+1) );
    REQUIRE( !uitsl::audit_greater(i, i) );
    REQUIRE( uitsl::audit_greater(i+1, i) );
    REQUIRE( uitsl::audit_greater(i+2, i) );
    REQUIRE( !uitsl::audit_greater(i, (int) 100) );
    REQUIRE( !uitsl::audit_greater( (long) -101, i) );
  }

  // ints
  for (int i = -100; i < 100; ++i) {
    REQUIRE( !uitsl::audit_greater(i, i+2) );
    REQUIRE( !uitsl::audit_greater(i, i+1) );
    REQUIRE( !uitsl::audit_greater(i, i) );
    REQUIRE( uitsl::audit_greater(i+1, i) );
    REQUIRE( uitsl::audit_greater(i+2, i) );
    REQUIRE( !uitsl::audit_greater(i, (short) 100) );
    REQUIRE( !uitsl::audit_greater( (short) -101, i) );
  }

}

TEST_CASE("Test audit_greater<unsigned, unsigned>", "[nproc:1]") {

  // big size_t
  for (
    size_t i = std::numeric_limits<size_t>::max() - 99;
    i < std::numeric_limits<size_t>::max();
    ++i
  ) {
    REQUIRE( !uitsl::audit_greater(i, i+1) );
    REQUIRE( !uitsl::audit_greater(i, i) );
    REQUIRE( uitsl::audit_greater(i+1, i) );
    REQUIRE( !uitsl::audit_greater((unsigned char) 0, i) );
  }

  // size_t wraparound
  REQUIRE( !uitsl::audit_greater( (size_t) 0,  (size_t) -1) );
  REQUIRE( !uitsl::audit_greater( (size_t) 0,  (size_t) 1) );
  REQUIRE( uitsl::audit_greater( (size_t) -1,  (size_t) 0) );
  REQUIRE( uitsl::audit_greater( (size_t) 1,  (size_t) 0) );
  REQUIRE( !uitsl::audit_greater( (size_t) -1,  (size_t) -1) );
  REQUIRE( !uitsl::audit_greater( (size_t) 0,  (size_t) 0) );

  // small size_t
  for (size_t i = 0; i < 99; ++i) {
    REQUIRE( !uitsl::audit_greater(i, i+1) );
    REQUIRE( !uitsl::audit_greater(i, i) );
    REQUIRE( uitsl::audit_greater(i+1, i) );
    REQUIRE( !uitsl::audit_greater(i, (unsigned char) 100) );
  }

  // unsigned char
  for (unsigned char i = 0; i < 99; ++i) {
    REQUIRE( !uitsl::audit_greater(i, i+1) );
    REQUIRE( !uitsl::audit_greater(i, i) );
    REQUIRE( uitsl::audit_greater(i+1, i) );
    REQUIRE( !uitsl::audit_greater(i, (size_t) 100) );
  }

}

TEST_CASE("Test audit_greater<signed, unsigned>", "[nproc:1]") {

  REQUIRE( !uitsl::audit_greater( (short) -1, (size_t) 0) );
  REQUIRE( !uitsl::audit_greater( (int) -1, (size_t) 0) );
  REQUIRE( !uitsl::audit_greater( (int) 0, (size_t) 1) );

  #ifndef NDEBUG
  REQUIRE_THROWS( !uitsl::audit_greater( (int) 0, (size_t) -1) );
  #endif
  REQUIRE( uitsl::audit_greater( (int) 1, (unsigned short) 0) );
  REQUIRE( uitsl::audit_greater( (int) 2, (size_t) 1) );
  REQUIRE( uitsl::audit_greater( (short) 42, (size_t) 25) );


  #ifndef NDEBUG
  REQUIRE_THROWS( !uitsl::audit_greater( (int) -1, (unsigned short) -1) );
  #endif
  REQUIRE( !uitsl::audit_greater( (short) 0, (size_t) 0) );
  REQUIRE( !uitsl::audit_greater( (int) 1, (size_t) 1) );

}

TEST_CASE("Test audit_greater<unsigned, signed>", "[nproc:1]") {

  REQUIRE( uitsl::audit_greater( (size_t) 0, (short) -1) );
  REQUIRE( uitsl::audit_greater( (size_t) 0, (int) -1) );
  REQUIRE( uitsl::audit_greater( (size_t) 1, (int) 0) );

  #ifndef NDEBUG
  REQUIRE_THROWS( uitsl::audit_greater( (size_t) -1, (int) 0) );
  #endif
  REQUIRE( !uitsl::audit_greater( (unsigned short) 0, (int) 1) );
  REQUIRE( !uitsl::audit_greater( (size_t) 1, (int) 2) );
  REQUIRE( !uitsl::audit_greater( (size_t) 25, (short) 42) );

  #ifndef NDEBUG
  REQUIRE_THROWS( uitsl::audit_greater( (unsigned short) -1, (int) -1) );
  #endif
  REQUIRE( !uitsl::audit_greater( (size_t) 0, (short) 0) );
  REQUIRE( !uitsl::audit_greater( (size_t) 1, (int) 1) );

}

TEST_CASE("Test audit_equal<signed, signed>", "[nproc:1]") {

  // shorts
  for (short i = -100; i < 100; ++i) {
    REQUIRE( !uitsl::audit_equal(i, i+2) );
    REQUIRE( !uitsl::audit_equal(i, i+1) );
    REQUIRE( uitsl::audit_equal(i, i) );
    REQUIRE( !uitsl::audit_equal(i+1, i) );
    REQUIRE( !uitsl::audit_equal(i+2, i) );
    REQUIRE( !uitsl::audit_equal(i, (int) 100) );
    REQUIRE( !uitsl::audit_equal( (long) -101, i) );
  }

  // ints
  for (int i = -100; i < 100; ++i) {
    REQUIRE( !uitsl::audit_equal(i, i+2) );
    REQUIRE( !uitsl::audit_equal(i, i+1) );
    REQUIRE( uitsl::audit_equal(i, i) );
    REQUIRE( !uitsl::audit_equal(i+1, i) );
    REQUIRE( !uitsl::audit_equal(i+2, i) );
    REQUIRE( !uitsl::audit_equal(i, (short) 100) );
    REQUIRE( !uitsl::audit_equal( (short) -101, i) );
  }

}

TEST_CASE("Test audit_equal<unsigned, unsigned>", "[nproc:1]") {

  REQUIRE(
    !uitsl::audit_equal( (unsigned int) -1, (unsigned short) -1)
  );
  REQUIRE(
    !uitsl::audit_equal( (unsigned short) -1, (unsigned int) -1)
  );

  // big size_t
  for (
    size_t i = std::numeric_limits<size_t>::max() - 99;
    i < std::numeric_limits<size_t>::max();
    ++i
  ) {
    REQUIRE( !uitsl::audit_equal(i, i+1) );
    REQUIRE( uitsl::audit_equal(i, i) );
    REQUIRE( !uitsl::audit_equal(i+1, i) );
    REQUIRE( !uitsl::audit_equal((unsigned char) 0, i) );
  }

  // size_t wraparound
  REQUIRE( !uitsl::audit_equal( (size_t) 0,  (size_t) -1) );
  REQUIRE( !uitsl::audit_equal( (size_t) 0,  (size_t) 1) );
  REQUIRE( !uitsl::audit_equal( (size_t) -1,  (size_t) 0) );
  REQUIRE( !uitsl::audit_equal( (size_t) 1,  (size_t) 0) );
  REQUIRE( uitsl::audit_equal( (size_t) -1,  (size_t) -1) );
  REQUIRE( uitsl::audit_equal( (size_t) 0,  (size_t) 0) );

  // small size_t
  for (size_t i = 0; i < 99; ++i) {
    REQUIRE( !uitsl::audit_equal(i, i+1) );
    REQUIRE( uitsl::audit_equal(i, i) );
    REQUIRE( !uitsl::audit_equal(i+1, i) );
    REQUIRE( !uitsl::audit_equal(i, (unsigned char) 100) );
  }

  // unsigned char
  for (unsigned char i = 0; i < 99; ++i) {
    REQUIRE( !uitsl::audit_equal(i, i+1) );
    REQUIRE( uitsl::audit_equal(i, i) );
    REQUIRE( !uitsl::audit_equal(i+1, i) );
    REQUIRE( !uitsl::audit_equal(i, (size_t) 100) );
  }

}

TEST_CASE("Test audit_equal<signed, unsigned>", "[nproc:1]") {

  REQUIRE(
    uitsl::audit_equal( (int) -1, (short) -1)
  );
  REQUIRE(
    uitsl::audit_equal( (short) -1, (int) -1)
  );

  #ifndef NDEBUG
  REQUIRE_THROWS(
    !uitsl::audit_equal( (short) -1, std::numeric_limits<unsigned short>::max())
  );
  REQUIRE_THROWS(
    !uitsl::audit_equal( (short) -1, std::numeric_limits<unsigned int>::max())
  );
  REQUIRE_THROWS(
    !uitsl::audit_equal( (int) -2, std::numeric_limits<unsigned int>::max() - 1)
  );
  #endif
  REQUIRE(
    !uitsl::audit_equal( (int) -2, std::numeric_limits<unsigned short>::max()-1)
  );

  REQUIRE( !uitsl::audit_equal( (short) -1, (size_t) 0) );
  REQUIRE( !uitsl::audit_equal( (int) -1, (size_t) 0) );
  REQUIRE( !uitsl::audit_equal( (int) 0, (size_t) 1) );

  #ifndef NDEBUG
  REQUIRE_THROWS( !uitsl::audit_equal( (int) 0, (size_t) -1) );
  #endif
  REQUIRE( !uitsl::audit_equal( (int) 1, (unsigned short) 0) );
  REQUIRE( !uitsl::audit_equal( (int) 2, (size_t) 1) );
  REQUIRE( !uitsl::audit_equal( (short) 42, (size_t) 25) );

  #ifndef NDEBUG
  REQUIRE_THROWS( !uitsl::audit_equal( (int) -1, (unsigned short) -1) );
  #endif
  REQUIRE( uitsl::audit_equal( (short) 0, (size_t) 0) );
  REQUIRE( uitsl::audit_equal( (int) 1, (size_t) 1) );

}

TEST_CASE("Test audit_equal<unsigned, signed>", "[nproc:1]") {

  #ifndef NDEBUG
  REQUIRE_THROWS(
    !uitsl::audit_equal( std::numeric_limits<unsigned short>::max(), (short) -1)
  );
  REQUIRE_THROWS(
    !uitsl::audit_equal( std::numeric_limits<unsigned int>::max(), (short) -1)
  );
  REQUIRE_THROWS(
    !uitsl::audit_equal( std::numeric_limits<unsigned int>::max() - 1, (int) -2)
  );
  #endif
  REQUIRE(
    !uitsl::audit_equal(std::numeric_limits<unsigned short>::max() - 1, (int)-2)
  );

  REQUIRE( !uitsl::audit_equal( (size_t) 0, (short) -1) );
  REQUIRE( !uitsl::audit_equal( (size_t) 0, (int) -1) );
  REQUIRE( !uitsl::audit_equal( (size_t) 1, (int) 0) );

  #ifndef NDEBUG
  REQUIRE_THROWS( !uitsl::audit_equal( (size_t) -1, (int) 0) );
  #endif
  REQUIRE( !uitsl::audit_equal( (unsigned short) 0, (int) 1) );
  REQUIRE( !uitsl::audit_equal( (size_t) 1, (int) 2) );
  REQUIRE( !uitsl::audit_equal( (size_t) 25, (short) 42) );

  #ifndef NDEBUG
  REQUIRE_THROWS( !uitsl::audit_equal( (unsigned short) -1, (int) -1) );
  #endif
  REQUIRE( uitsl::audit_equal( (size_t) 0, (short) 0) );
  REQUIRE( uitsl::audit_equal( (size_t) 1, (int) 1) );

}
