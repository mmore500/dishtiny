#include <array>
#include <ratio>
#include <variant>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/binary.hpp"
#include "cereal/include/cereal/cereal.hpp"
#include "cereal/include/cereal/types/array.hpp"
#include "cereal/include/cereal/types/optional.hpp"
#include "cereal/include/cereal/types/variant.hpp"
#include "Empirical/include/emp/base/optional.hpp"
#include "Empirical/include/emp/io/ContiguousStream.hpp"


using small_t = size_t;
using big_t = std::array<size_t, std::kilo::num>;

TEST_CASE("Test variant", "[nproc:1]") {

  using variant_t = std::variant<small_t, big_t>;

  emp::ContiguousStream cs_small;
  emp::ContiguousStream cs_big;

  { // oarchive flushes on destruction
    cereal::BinaryOutputArchive oarchive(
      cs_small
    );
    oarchive( variant_t{ std::in_place_type_t<small_t>{} } );
  }

  { // oarchive flushes on destruction
    cereal::BinaryOutputArchive oarchive(
      cs_big
    );
    oarchive( variant_t{ std::in_place_type_t<big_t>{} } );
  }

  REQUIRE( cs_small.GetSize() <  cs_big.GetSize() );

}

TEST_CASE("Test optional", "[nproc:1]") {

  emp::ContiguousStream cs_small;
  emp::ContiguousStream cs_big;

  { // oarchive flushes on destruction
    cereal::BinaryOutputArchive oarchive(
      cs_small
    );
    oarchive( emp::optional<big_t>{} );
  }

  { // oarchive flushes on destruction
    cereal::BinaryOutputArchive oarchive(
      cs_big
    );
    oarchive( emp::optional<big_t>{ big_t{} } );
  }

  REQUIRE( cs_small.GetSize() <  cs_big.GetSize() );

}
