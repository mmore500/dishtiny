#pragma once
#ifndef UITSL_MATH_MATH_UTILS_HPP_INCLUDE
#define UITSL_MATH_MATH_UTILS_HPP_INCLUDE

#include <bitset>
#include <cmath>
#include <limits>
#include <ratio>
#include <stddef.h>

#include "../../../third-party/Empirical/include/emp/math/math.hpp"

#include "../debug/safe_cast.hpp"

namespace uitsl {

template<typename ...Args> auto sum(Args ...args) { return (args + ...); }

inline size_t mod(int in_val, const size_t mod_val) {
  emp_assert(mod_val > 0);
  const int signed_mod_val = std::min(
    safe_cast<size_t>(std::numeric_limits<int>::max()),
    mod_val
  );
  in_val %= signed_mod_val;
  return (in_val < 0) ? (in_val + signed_mod_val) : in_val;
}

inline size_t circular_index(const size_t pos, const size_t len, const int diff) {
  emp_assert(len > 0);
  return (pos + mod(diff, len)) % len;
}

inline size_t stoszt(const std::string & source) {
  std::stringstream ss{source};
  size_t res;
  ss >> res;
  return res;
}

template<typename TYPE>
constexpr bool is_power_of_two(TYPE x) {
  return x > 0 && emp::CountOnes(x) == 1;
}

template<typename TYPE>
constexpr int num_bits(TYPE x) {
  return x ? emp::IntLog2(emp::Abs(x)) + 1 : 0;
}

template<typename TYPE>
bool test_bit(const TYPE x, const size_t bit) {
  return std::bitset<sizeof(TYPE)*8>(emp::Abs(x)).test(bit);
}

inline size_t difference(const size_t a, const size_t b) {
  return std::max(a, b) - std::min(a, b);
}

template<typename BottomFrac = std::ratio<1, 2> >
size_t sidebyside_hash(const size_t top, const size_t bottom) {

  constexpr size_t int_size = sizeof(int) * 8;
  // exclude int sign bit
  constexpr size_t avail_size = int_size - 1;
  constexpr size_t bottom_size = avail_size * BottomFrac::num / BottomFrac::den;
  [[maybe_unused]] constexpr size_t top_size = avail_size - bottom_size;

  // bounds checking
  emp_assert(std::bitset<top_size>(top).to_ullong() == top, top);
  emp_assert(std::bitset<bottom_size>(bottom).to_ullong() == bottom, bottom);

  const auto bottom_bits = std::bitset<int_size>( bottom );
  const auto top_bits = std::bitset<int_size>( top ) << bottom_size;

  const auto res = top_bits | bottom_bits;

  return res.to_ullong();

}

} // namespace uitsl

#endif // #ifndef UITSL_MATH_MATH_UTILS_HPP_INCLUDE
