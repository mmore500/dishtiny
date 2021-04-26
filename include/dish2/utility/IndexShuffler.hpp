#pragma once
#ifndef DISH2_UTILITY_INDEXSHUFFLER_HPP_INCLUDE
#define DISH2_UTILITY_INDEXSHUFFLER_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

class IndexShuffler {

  emp::vector<size_t> data;

public:

  IndexShuffler() = default;

  IndexShuffler( const size_t size ) { Resize( size ); }

  void Resize( const size_t size ) {
    if ( size != data.size() ) {
      data.resize( size );
      std::iota(
        std::begin( data ), std::end( data ),
        size_t{}
      );
    }
  }

  void Shuffle() { emp::Shuffle( sgpl::tlrand.Get(), data ); }

  auto begin() const { return std::begin( data ); }

  auto end() const { return std::end( data ); }

};

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_INDEXSHUFFLER_HPP_INCLUDE
