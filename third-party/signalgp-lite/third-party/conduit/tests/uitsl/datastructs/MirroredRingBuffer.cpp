#include <ratio>
#include <vector>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/datastructs/MirroredRingBuffer.hpp"

TEST_CASE("Test MirroredRingBuffer", "[nproc:1]") {

  constexpr size_t buff_size{ 10 };
  uitsl::MirroredRingBuffer<size_t, buff_size> buff;

  for (size_t rep = 0; rep < std::mega::num; ++rep) {

    REQUIRE( buff.GetSize() == 0 );

    for (size_t i = 0; i < buff_size; ++i) {
      REQUIRE( buff.GetSize() == i );
      REQUIRE( buff.PushHead(rep + i) );
    }
    REQUIRE( buff.GetSize() == buff_size );
    REQUIRE( !buff.PushHead(0) );

    for (size_t i = 0; i < buff_size; ++i) {
      REQUIRE( buff.Get(i) == rep + i );
    }

    std::vector<size_t> expected( buff_size );
    std::iota(
      std::begin(expected),
      std::end(expected),
      rep
    );
    for (size_t i = 0; i < buff_size; ++i) REQUIRE( expected[i] == rep + i );

    REQUIRE( 0 == memcmp(
      buff.GetTailPtr(),
      expected.data(),
      buff.GetSize() * sizeof(size_t)
    ) );

    for (size_t i = 0; i < buff_size; ++i) {
      REQUIRE( buff.GetSize() == buff_size - i );
      REQUIRE( buff.PopTail() );
    }

    REQUIRE( buff.GetSize() == 0 );
    REQUIRE( !buff.PopTail() );
  }

}
