#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/include/emp/bits/BitSet.hpp"
#include "Empirical/include/emp/math/Random.hpp"

#include "uitsl/algorithm/unset_mask.hpp"

TEST_CASE("Test unset_mask random testing", "[nproc:1]") {

  emp::Random rand(1);

  const emp::BitSet<64> target( rand );
  const emp::BitSet<64> mask( rand );

  const auto after_apply = uitsl::unset_mask( target, mask );

  REQUIRE( (after_apply & mask) == emp::BitSet<64>{} );

  REQUIRE(
    (mask | after_apply).CountOnes() == mask.CountOnes() + after_apply.CountOnes()
  );

  REQUIRE( (target | after_apply) == target );
  REQUIRE( (mask | after_apply).CountOnes() >= target.CountOnes() );
  REQUIRE( ( (mask&target) | after_apply) == target );

}

TEST_CASE("Test unset_mask hand testing", "[nproc:1]") {

  const emp::BitSet<4> target{1,1,0,0};
  const emp::BitSet<4> mask{0,1,1,0};

  const auto after_apply = uitsl::unset_mask( target, mask );

  REQUIRE( after_apply == emp::BitSet<4>{1,0,0,0} );

}
