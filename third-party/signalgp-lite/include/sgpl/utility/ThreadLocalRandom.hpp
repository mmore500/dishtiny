#pragma once
#ifndef SGPL_UTILITY_THREADLOCALRANDOM_HPP_INCLUDE
#define SGPL_UTILITY_THREADLOCALRANDOM_HPP_INCLUDE

#include <cstddef>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"

namespace sgpl {

namespace internal {

class ThreadLocalRandom {

  emp::Random rand{ 1 };

  uint32_t cache;

  std::byte* cache_ptr{
    reinterpret_cast<std::byte*>( &cache )
  };

  size_t cache_pos{ sizeof(cache) };

public:

  emp::Random& Get() { return rand; }

  std::byte GetByte() {

    if ( cache_pos == sizeof( cache ) ) {
      cache_pos %= sizeof( cache );
      cache = Get().GetUInt();
    }

    const auto res { cache_ptr[cache_pos] };
    ++cache_pos;
    return res;

  }

  void Reseed( const int seed ) {

    // seed <= 0 non-deterministic (uses system time and memory address)
    emp_assert( seed > 0 );

    cache_pos = sizeof( cache );

    Get() = emp::Random{ seed };

  }

  void Initialize( const int seed ) {

    // assert that rng hasn't been touched already
    emp_assert( Get().GetUInt() == emp::Random{ 1 }.GetUInt() );

    Reseed( seed );

  }

  void SeedStochastically() { Get() = emp::Random{ -1 }; }

};

} // namespace internal

thread_local sgpl::internal::ThreadLocalRandom tlrand{};

} // namespace sgpl

#endif // #ifndef SGPL_UTILITY_THREADLOCALRANDOM_HPP_INCLUDE
