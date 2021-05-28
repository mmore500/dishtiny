#pragma once
#ifndef DISH2_UTILITY_SHA256_FILE_HPP_INCLUDE
#define DISH2_UTILITY_SHA256_FILE_HPP_INCLUDE

#include <cstddef>
#include <cstring>
#include <streambuf>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/PicoSHA2/picosha2.h"

#include "BytewiseIterator.hpp"

namespace dish2 {

template<typename H>
uint32_t sha256_file(H handle) {

  // we'll use first sizeof( uint32_t ) bytes as the result
  static_assert( picosha2::k_digest_size >= sizeof( uint32_t ) );

  emp::array<unsigned char, picosha2::k_digest_size> hash;

  picosha2::hash256(
    std::istreambuf_iterator<char>(handle), std::istreambuf_iterator<char>(),
    std::begin( hash ), std::end( hash )
  );

  uint32_t res;
  std::memcpy( &res, hash.data(), sizeof( uint32_t ) );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_SHA256_FILE_HPP_INCLUDE
