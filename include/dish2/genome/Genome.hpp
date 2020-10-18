#pragma once
#ifndef DISH2_GENOME_GENOME_HPP_INCLUDE
#define DISH2_GENOME_GENOME_HPP_INCLUDE

#include <algorithm>
#include <cstddef>

#include "../../../third-party/Empirical/source/polyfill/span.h"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

template<typename Spec>
struct Genome {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  sgpl::Program<sgpl_spec_t> program;

  Genome() = default;

  Genome(const size_t len) : program(len) {}

  bool operator==(const Genome& other) const {
    return program == other.program;
  }

  template <class Archive>
  void serialize( Archive & ar ) {
    ar( program );
  }

};

} // namespace dish2

namespace std {

template <typename Spec>
struct hash<dish2::Genome<Spec>> {

size_t operator()( const dish2::Genome<Spec>& genome ) const {
  const auto& program = genome.program;
  return emp::murmur_hash( std::span<const std::byte>(
    reinterpret_cast<const std::byte*>( program.data() ),
    // TODO only bother hashing up to the first 32 bytes?
    // std::min( program.size() * sizeof( program.front() ), 32ul )
    program.size() * sizeof( program.front() )
  ) );
}

};

} // namespace std

#endif // #ifndef DISH2_GENOME_GENOME_HPP_INCLUDE
