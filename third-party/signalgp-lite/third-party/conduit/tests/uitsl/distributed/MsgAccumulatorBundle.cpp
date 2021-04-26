#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/distributed/MsgAccumulatorBundle.hpp"
#include "uitsl/polyfill/identity.hpp"


TEST_CASE("test MsgAccumulatorBundle") {
  SECTION("Test data") {
    emp::vector<int> src{1, 2, 3};

    uitsl::MsgAccumulatorBundle<int> dest{ src };

    REQUIRE( 0 == std::memcmp(
      src.data(),
      dest.data(),
      src.size() * sizeof(int)
    ) );

    dest.BumpEpoch();

    size_t res;
    std::memcpy(
      &res,
      dest.data() + src.size(),
      sizeof(size_t)
    );

    REQUIRE( res == 1 );
  }

  SECTION("test  byte_size") {
    emp::vector<int> src{1, 2, 3};

    uitsl::MsgAccumulatorBundle<int> dest{ src };

    REQUIRE( dest.byte_size() == src.size() * sizeof(int) + sizeof(size_t) );
  }

  SECTION("test GetData") {
    emp::vector<double> src{1.0, 2.0, 3.0};

    uitsl::MsgAccumulatorBundle<double> dest{ src };

    REQUIRE( dest.GetData() == src );
  }

  SECTION("test Reset") {
    emp::vector<double> src{1.0, 2.0, 3.0};

    uitsl::MsgAccumulatorBundle<double> dest{ src };

    dest.Reset();

    REQUIRE( std::none_of(
      reinterpret_cast<char*>( dest.data() ),
      reinterpret_cast<char*>( dest.data() ) + dest.byte_size(),
      std::identity
    ) );
  }

  SECTION("test GetEpoch, BumpEpoch, and SetEpoch") {
    emp::vector<double> src{1.0, 2.0, 3.0};

    uitsl::MsgAccumulatorBundle<double> dest{ src };

    REQUIRE( dest.GetEpoch() == 0 );

    dest.BumpEpoch();
    REQUIRE( dest.GetEpoch() == 1 );
    REQUIRE( 0 == std::memcmp(
      src.data(),
      dest.data(),
      src.size() * sizeof(int)
    ) );

    dest.BumpEpoch();
    REQUIRE( dest.GetEpoch() == 2 );
    REQUIRE( 0 == std::memcmp(
      src.data(),
      dest.data(),
      src.size() * sizeof(int)
    ) );

    dest.BumpEpoch();
    REQUIRE( dest.GetEpoch() == 3 );
    REQUIRE( 0 == std::memcmp(
      src.data(),
      dest.data(),
      src.size() * sizeof(int)
    ) );

    dest.SetEpoch(42);
    REQUIRE( dest.GetEpoch() == 42 );
    REQUIRE( 0 == std::memcmp(
      src.data(),
      dest.data(),
      src.size() * sizeof(int)
    ) );

    dest.Reset();
    REQUIRE( dest.GetEpoch() == 0 );

    dest.SetEpoch(42);
    REQUIRE( dest.GetEpoch() == 42 );

  }

  SECTION("test BumpData") {
    emp::vector<double> src{1.0, 2.0, 3.0};

    uitsl::MsgAccumulatorBundle<double> dest{ src };
    dest.BumpData( src );

    REQUIRE( dest.GetData() == emp::vector<double>{2.0, 4.0, 6.0} );
  }

SECTION("test operator+=") {
    emp::vector<double> src{1.0, 2.0, 3.0};

    uitsl::MsgAccumulatorBundle<double> dest{ src };
    dest.BumpData( src );
    dest.BumpData( src );
    dest.BumpEpoch();

    dest += dest;

    REQUIRE( dest.GetEpoch() == 2 );
    REQUIRE( dest.GetData() == emp::vector<double>{6.0, 12.0, 18.0} );
  }
}
