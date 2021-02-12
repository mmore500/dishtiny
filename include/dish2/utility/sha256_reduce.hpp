#pragma once
#ifndef DISH2_UTILITY_SHA256_REDUCE_HPP_INCLUDE
#define DISH2_UTILITY_SHA256_REDUCE_HPP_INCLUDE

#include <cstddef>
#include <cstring>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"
#include "../../../third-party/PicoSHA2/picosha2.h"

#include "BytewiseIterator.hpp"

namespace dish2 {

template<typename It>
uint32_t sha256_reduce(const It begin, const It end) {

  // we'll use first sizeof( uint32_t ) bytes as the result
  static_assert( picosha2::k_digest_size >= sizeof( uint32_t ) );

  emp::array<unsigned char, picosha2::k_digest_size> hash;

  using bytewise_wrapper_t = dish2::BytewiseIterator<It>;
  picosha2::hash256(
    bytewise_wrapper_t{ begin }, bytewise_wrapper_t{ end },
    std::begin(hash), std::end(hash)
  );

  uint32_t res;
  std::memcpy( &res, hash.data(), sizeof( uint32_t ) );

  return res;

}

template<typename T>
uint32_t sha256_reduce(const T& container) {

  return dish2::sha256_reduce( std::begin( container ), std::end( container ) );

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_SHA256_REDUCE_HPP_INCLUDE
