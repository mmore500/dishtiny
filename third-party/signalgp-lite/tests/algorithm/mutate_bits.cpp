#include <algorithm>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/array.hpp"

#include "sgpl/algorithm/mutate_bits.hpp"

TEST_CASE("Test mutate_bits 0 -> 1") {

  emp::array<std::byte, 1024> buff{};

  REQUIRE( std::all_of(
    std::begin( buff ),
    std::end( buff ),
    [](const auto& byte){ return byte == std::byte{}; }
  ) );

  sgpl::mutate_bits( std::span<std::byte>(buff.data(), 1024), 1 );

  REQUIRE( std::any_of(
    std::begin( buff ),
    std::end( buff ),
    [](const auto& byte){ return byte != std::byte{}; }
  ) );

  sgpl::mutate_bits( std::span<std::byte>(buff.data(), 1024), 100 );

  REQUIRE( std::count_if(
    std::begin( buff ),
    std::end( buff ),
    [](const auto& byte){ return byte != std::byte{}; }
  ) > 20 );

}

TEST_CASE("Test mutate_bits 1 -> 0") {

  emp::array<std::byte, 1024> buff{};
  std::fill( std::begin( buff ), std::end( buff ), ~std::byte{} );

  REQUIRE( std::all_of(
    std::begin( buff ),
    std::end( buff ),
    [](const auto& byte){ return byte == ~std::byte{}; }
  ) );

  sgpl::mutate_bits( std::span<std::byte>(buff.data(), 1024), 1 );

  REQUIRE( std::any_of(
    std::begin( buff ),
    std::end( buff ),
    [](const auto& byte){ return byte != ~std::byte{}; }
  ) );

  sgpl::mutate_bits( std::span<std::byte>(buff.data(), 1024), 100 );

  REQUIRE( std::count_if(
    std::begin( buff ),
    std::end( buff ),
    [](const auto& byte){ return byte != ~std::byte{}; }
  ) > 20 );

}
