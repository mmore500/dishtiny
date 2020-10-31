#pragma once
#ifndef DISH2_GENOME_EVENTTAGS_HPP_INCLUDE
#define DISH2_GENOME_EVENTTAGS_HPP_INCLUDE

#include "../../../third-party/Empirical/source/base/array.h"
#include "../../../third-party/Empirical/source/tools/BitSet.h"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/mutate_bytes.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

template<typename Spec>
struct EventTags {

  using tags_t = emp::array< emp::BitSet<32> , Spec::NUM_EVENTS >;
  tags_t tags;

  constexpr inline static size_t tag_bytes
    = sizeof( typename tags_t::value_type ) * Spec::NUM_EVENTS;

  EventTags() = default;

  EventTags( std::in_place_t ) {
    sgpl::ThreadLocalRandom::Get().RandFill(
      reinterpret_cast<unsigned char*>( tags.data() ),
      tag_bytes
    );
  }

  size_t ApplyPointMutations( const float p_byte_scramble ) {

    // ideally, we would draw from the binomial distn,
    // but that's expensive with varying n...
    // so approximate with the poisson distribution instead
    // they're similar-ish, e.g., https://www.researchgate.net/figure/Poisson-versus-binomial-distribution-from-number-of-heads-in-a-coin-toss-The-Poisson_fig3_255717571
    // (they become more similar for large n)
    const size_t n_muts = sgpl::ThreadLocalRandom::Get().GetRandPoisson(
      tag_bytes,
      p_byte_scramble
    );

    sgpl::mutate_bytes(
      std::span<std::byte>(
        reinterpret_cast<std::byte*>( tags.data() ),
        tag_bytes
      ),
      n_muts
    );

    return n_muts;

  }

  bool operator==(const EventTags& other) const { return tags == other.tags; }

  template<typename Archive> void serialize( Archive & ar ) { ar( tags ); }

};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_EVENTTAGS_HPP_INCLUDE
