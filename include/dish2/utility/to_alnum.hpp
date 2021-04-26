#pragma once
#ifndef DISH2_UTILITY_TO_ALNUM_HPP_INCLUDE
#define DISH2_UTILITY_TO_ALNUM_HPP_INCLUDE

#include <cstddef>
#include <cstring>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"
#include "../../../third-party/PicoSHA2/picosha2.h"

namespace dish2 {

std::string to_alnum(size_t i) {

  static const char alnum[]
    = "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";
  const size_t len = sizeof(alnum) - 1;

  std::string res;
  res.reserve( sizeof( size_t ) * 2 );

  while ( i ) {
    res.push_back( alnum[i % len] );
    i /= len;
  }

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_TO_ALNUM_HPP_INCLUDE
