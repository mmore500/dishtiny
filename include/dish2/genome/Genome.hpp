#pragma once
#ifndef DISH2_GENOME_GENOME_HPP_INCLUDE
#define DISH2_GENOME_GENOME_HPP_INCLUDE

#include <algorithm>
#include <cstddef>
#include <tuple>
#include <utility>

#include "../../../third-party/Empirical/source/polyfill/span.h"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/sloppy_copy.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../config/cfg.hpp"

#include "EventTags.hpp"
#include "GenerationCounter.hpp"
#include "Genome.hpp"
#include "KinGroupID.hpp"
#include "MutationCounter.hpp"
#include "RootID.hpp"

namespace dish2 {

template<typename Spec>
struct Genome {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  using event_tags_t = dish2::EventTags<Spec>;

  event_tags_t event_tags;
  dish2::GenerationCounter<Spec> generation_counter;
  dish2::KinGroupID<Spec> kin_group_id;
  dish2::MutationCounter mutation_counter;
  using program_t = sgpl::Program<sgpl_spec_t>;
  program_t program;
  dish2::RootID root_id;

  Genome() = default;

  Genome(std::in_place_t)
  : event_tags( std::in_place )
  , kin_group_id( std::in_place )
  , program( dish2::cfg.PROGRAM_START_SIZE() )
  , root_id( std::in_place ) {}

  bool operator==(const Genome& other) const {
    return std::tuple{
      event_tags,
      generation_counter,
      kin_group_id,
      mutation_counter,
      program,
      root_id
    } == std::tuple{
      other.event_tags,
      other.generation_counter,
      other.kin_group_id,
      other.mutation_counter,
      other.program,
      other.root_id
    };
  }

  void ElapseGeneration(const size_t rep_lev) {

    generation_counter.ElapseGeneration( rep_lev );
    kin_group_id.ApplyInheritance( rep_lev );

    if ( sgpl::ThreadLocalRandom::Get().P( dish2::cfg.MUTATION_RATE() ) ) {
      MutateProgram();
      mutation_counter.RecordPointMutation(
        event_tags.ApplyPointMutations( dish2::cfg.POINT_MUTATION_RATE() )
      );
    }

    // root_id doesn't change

  }

  void MutateProgram() {
    DoProgramSequenceMutation();
    mutation_counter.RecordPointMutation( program.ApplyPointMutations(
      dish2::cfg.POINT_MUTATION_RATE()
    ) );
  }

  void DoProgramSequenceMutation() {
    using inst_t = typename program_t::value_type;

    // TODO perform the sloppy copy elsewhere for efficiency's sake?
    if ( sgpl::ThreadLocalRandom::Get().P(
      dish2::cfg.SEVERE_SEQUENCE_MUTATION_RATE()
    ) ) {
      // do severe sequence mutation
      auto [copy, num_muts] = sgpl::sloppy_copy<inst_t, 0>(
        program,
        dish2::cfg.SEQUENCE_DEFECT_RATE(),
        program.size(),
        dish2::cfg.PROGRAM_MAX_SIZE()
      );
      mutation_counter.RecordInsertionDeletion( num_muts );
      program = std::move( copy );
    } else {
      // do minor sequence mutation
      auto [copy, num_muts] = sgpl::sloppy_copy<inst_t, 1>(
        program,
        dish2::cfg.SEQUENCE_DEFECT_RATE(),
        dish2::cfg.MINOR_SEQUENCE_MUTATION_BOUND(),
        dish2::cfg.PROGRAM_MAX_SIZE()
      );
      mutation_counter.RecordInsertionDeletion( num_muts );
      program = std::move( copy );
    }
  }

  template <class Archive>
  void serialize( Archive & ar ) { ar(
    event_tags,
    generation_counter,
    mutation_counter,
    kin_group_id,
    program,
    root_id
  ); }

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
