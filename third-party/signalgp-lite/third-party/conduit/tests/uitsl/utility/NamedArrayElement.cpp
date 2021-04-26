#include <limits>
#include <utility>

#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/include/emp/base/array.hpp"

#include "uitsl/utility/NamedArrayElement.hpp"

// adapted from http://cplusplus.bordoon.com/namedArrayElements.html
struct NamedArray {

  union {

    double m_array[2];

    uitsl::NamedArrayElement<0, double> foo;
    uitsl::NamedArrayElement<1, double> bar;

  };

  double &operator[](const int i) { return m_array[i]; }

  const double &operator[](const int i) const { return m_array[i]; }

};

TEST_CASE("NamedArrayElement", "[nproc:1]") {

  NamedArray arr;

  arr.foo = 42;
  arr.bar = 24;

  REQUIRE( arr[0] == 42 );
  REQUIRE( arr[1] == 24 );

  std::swap(arr[0], arr[1]);

  REQUIRE( arr[0] == 24 );
  REQUIRE( arr[1] == 42 );

}
