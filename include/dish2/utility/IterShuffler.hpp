#pragma once
#ifndef DISH2_UTILITY_ITERSHUFFLER_HPP_INCLUDE
#define DISH2_UTILITY_ITERSHUFFLER_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

template<typename T>
class IterShuffler {

  emp::vector<T> data;

public:

  IterShuffler() = default;

  IterShuffler( const T begin, const T end ) { Fill(begin, end); }

  void Fill( const T begin, const T end ) {
    data.resize( std::distance( begin, end ) );
    std::iota(
      std::begin( data ), std::end( data ),
      begin
    );
  }

  void Shuffle() { emp::Shuffle( sgpl::tlrand.Get(), data ); }

  auto begin() const { return std::begin( data ); }

  auto end() const { return std::end( data ); }

};

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_ITERSHUFFLER_HPP_INCLUDE
