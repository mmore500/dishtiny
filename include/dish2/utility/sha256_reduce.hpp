#pragma once
#ifndef DISH2_UTILITY_SHA256_REDUCE_HPP_INCLUDE
#define DISH2_UTILITY_SHA256_REDUCE_HPP_INCLUDE

#include <cstddef>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"
#include "../../../third-party/PicoSHA2/picosha2.h"


namespace dish2 {

template<typename It>
size_t sha256_reduce(const It begin, const It end) {

  static_assert( picosha2::k_digest_size >= 16 );

  emp::vector<unsigned char> hash( picosha2::k_digest_size );

  picosha2::hash256(begin, end, std::begin(hash), std::end(hash));

  return emp::murmur_hash( std::span< std::byte >(
    reinterpret_cast<std::byte*>( hash.data() ),
    hash.size()
  ) );

}

template<typename T>
size_t sha256_reduce(const T& container) {

  return dish2::sha256_reduce( std::begin( container ), std::end( container ) );

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_SHA256_REDUCE_HPP_INCLUDE
