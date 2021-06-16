#pragma once
#ifndef DISH2_GENOME_PHYLOFINGERPRINTS_HPP_INCLUDE
#define DISH2_GENOME_PHYLOFINGERPRINTS_HPP_INCLUDE

#include <algorithm>
#include <cmath>

#include "../polyfill/bit_floor.hpp"
#include "../polyfill/bit_width.hpp"
#include "../polyfill/countr_zero.hpp"

// #include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

class PhyloFingerprints {

  uint64_t generation_counter{};

  // ordered from most ancient (index 0 / fingerprints.front())
  // to most recent (index n - 1 / fingerprints.back())
  std::vector<uint64_t> fingerprints;
  // absolute generation corresponding to each fingerprint
  std::vector<uint64_t> generations;

// todo remove
public:

  // msb == most significant bit
  static uint64_t clear_msb(const uint64_t v) {
    return v - std::bit_floor(v);
  }

  // lsb == lowest set bit
  // aka (x & -x).set_width()
  // not defined for x == 0
  // == https://oeis.org/A001511 - 1
  static uint64_t get_lsb_index(const uint64_t v) {
    emp_assert(v);
    return std::countr_zero(v);
  }

  // 0 == no drop
  // should generate sequence
  // 0, 0, 1, 0, 1, 2, 1, 0, 1, 2, 1, 3, 1, 2, 1, 0, ...
  // non-zero interludes are from https://oeis.org/A001511
  // zeros occur when index is an even power of two
  static uint64_t calc_drop_idx(const uint64_t gen) {
    const uint64_t leftover = clear_msb(gen);
    if ( leftover ) return get_lsb_index(leftover) + 1;
    else return 0;
  }

  void append_fingerprint() {
    fingerprints.push_back(
      sgpl::tlrand.Get().GetUInt64()
    );
    generations.push_back(
      generation_counter
    );
  }

  void maybe_drop_fingerprint(const size_t drop_idx) {
    if ( drop_idx ) {
      fingerprints.erase(
        std::next( std::rbegin(fingerprints), drop_idx ).base()
      );
      generations.erase(
        std::next( std::rbegin(generations), drop_idx ).base()
      );
    }
  }

  void print() {
    for ( auto& g : generations ) std::cout << generation_counter - g << " ";
    std::cout << '\n';
  }

public:

  PhyloFingerprints() { append_fingerprint(); ++generation_counter; }

  void ElapseGeneration() {
    ++generation_counter;

    maybe_drop_fingerprint( calc_drop_idx(generation_counter) );

    append_fingerprint();
  }

  size_t DivergedAtLeast( const PhyloFingerprints& other ) const {
    const auto [our_mismatch, others_mismatch] = std::mismatch(
      std::begin( fingerprints ), std::end( fingerprints ),
      std::begin( other.fingerprints )
    );

    const size_t num_divergent_fingerprints = std::distance(
      our_mismatch, std::end( fingerprints )
    );

    if ( num_divergent_fingerprints ) {
      const size_t first_divergent_generation = generations[
        generations.size() - num_divergent_fingerprints
      ];
      return generation_counter - first_divergent_generation;
    } else return 0;

  }

  size_t DivergedLessThan( const PhyloFingerprints& other ) const {
    const auto [our_mismatch, others_mismatch] = std::mismatch(
      std::begin( fingerprints ), std::end( fingerprints ),
      std::begin( other.fingerprints )
    );

    const size_t num_common_fingerprints = std::distance(
      std::begin( fingerprints ), our_mismatch
    );

    if ( num_common_fingerprints ) {
      const size_t last_common_generation = generations[
        num_common_fingerprints - 1
      ];
      return generation_counter - last_common_generation;
    } else return -1;

  }

};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_PHYLOFINGERPRINTS_HPP_INCLUDE
