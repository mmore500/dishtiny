#pragma once
#ifndef SGPL_ALGORITHM_MUTATE_BITS_HPP_INCLUDE
#define SGPL_ALGORITHM_MUTATE_BITS_HPP_INCLUDE

#include <cstddef>
#include <limits>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"

#include "../utility/ThreadLocalRandom.hpp"

namespace sgpl {

void mutate_bits(const std::span<std::byte> target) {

  const size_t bit_idx = sgpl::tlrand.Get().GetUInt(
    target.size_bytes() * CHAR_BIT
  );

  const size_t byte_idx = bit_idx / CHAR_BIT;
  const size_t within_byte_idx = uitsl::shift_mod( bit_idx, CHAR_BIT );

  target[ byte_idx ] ^= ( std::byte{1} << within_byte_idx );

}


void mutate_bits(const std::span<std::byte> target, const size_t num_muts) {

  for (size_t i{}; i < num_muts; ++i) sgpl::mutate_bits(target);

}


} // namespace sgpl

#endif // #ifndef SGPL_ALGORITHM_MUTATE_BITS_HPP_INCLUDE
