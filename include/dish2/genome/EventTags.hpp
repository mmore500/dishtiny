#pragma once
#ifndef DISH2_GENOME_EVENTTAGS_HPP_INCLUDE
#define DISH2_GENOME_EVENTTAGS_HPP_INCLUDE

#include <cstddef>
#include <limits>

#include "../../../third-party/cereal/include/cereal/cereal.hpp"
#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/mutate_bits.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

template<typename Spec>
struct EventTags {

  using tag_t = typename Spec::tag_t;

  using tags_t = std::array< tag_t , Spec::NUM_EVENTS >;
  tags_t tags;

  constexpr inline static size_t tag_bytes
    = sizeof( typename tags_t::value_type ) * Spec::NUM_EVENTS;

  EventTags() = default;

  EventTags( std::in_place_t ) {
    sgpl::tlrand.Get().RandFill(
      reinterpret_cast<unsigned char*>( tags.data() ),
      tag_bytes
    );
  }

  size_t ApplyPointMutations( const float p_bit_toggle ) {

    // ideally, we would draw from the binomial distn,
    // but that's expensive with varying n...
    // so approximate with the poisson distribution instead
    // they're similar-ish, e.g., https://www.researchgate.net/figure/Poisson-versus-binomial-distribution-from-number-of-heads-in-a-coin-toss-The-Poisson_fig3_255717571
    // (they become more similar for large n)
    const size_t n_muts = sgpl::tlrand.Get().GetRandPoisson(
      tag_bytes * CHAR_BIT,
      p_bit_toggle
    );

    sgpl::mutate_bits(
      std::span<std::byte>(
        reinterpret_cast<std::byte*>( tags.data() ),
        tag_bytes
      ),
      n_muts
    );

    return n_muts;

  }

  bool operator==(const EventTags& other) const { return tags == other.tags; }

  bool operator<(const EventTags& other) const { return tags < other.tags; }

  template<typename Archive> void serialize( Archive & ar ) {
    ar( CEREAL_NVP( tags ) );
  }

};

} // namespace dish2

namespace std {

template <typename Spec>
struct hash<dish2::EventTags<Spec>> {

size_t operator()( const dish2::EventTags<Spec>& event_tags ) const {
  const auto& tags = event_tags.tags;
  return emp::murmur_hash( std::span<const std::byte>(
    reinterpret_cast<const std::byte*>( tags.data() ),
    tags.size() * sizeof( tags.front() )
  ) );
}

};

} // namespace std

#endif // #ifndef DISH2_GENOME_EVENTTAGS_HPP_INCLUDE
