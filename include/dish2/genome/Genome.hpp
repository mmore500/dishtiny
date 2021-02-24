#pragma once
#ifndef DISH2_GENOME_GENOME_HPP_INCLUDE
#define DISH2_GENOME_GENOME_HPP_INCLUDE

#include <algorithm>
#include <cstddef>
#include <tuple>
#include <utility>

#include "../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/sloppy_copy.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../config/cfg.hpp"

#include "EventTags.hpp"
#include "GenerationCounter.hpp"
#include "Genome.hpp"
#include "KinGroupEpochStamps.hpp"
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
  dish2::KinGroupEpochStamps<Spec> kin_group_epoch_stamps;
  dish2::KinGroupID<Spec> kin_group_id;
  dish2::MutationCounter mutation_counter;
  using program_t = sgpl::Program<sgpl_spec_t>;
  program_t program;
  dish2::RootID root_id;
  dish2::RootID stint_root_id;

  Genome() = default;

  Genome(std::in_place_t)
  : event_tags( std::in_place )
  , kin_group_id( std::in_place )
  , program( dish2::cfg.PROGRAM_START_SIZE() )
  , root_id( std::in_place )
  , stint_root_id( std::in_place ) {
    program.RotateGlobalAnchorToFront();
  }

  bool operator==(const Genome& other) const {
    // ignore kin_group_epoch_stamps,
    return std::tuple{
      event_tags,
      generation_counter,
      kin_group_id,
      mutation_counter,
      program,
      root_id,
      stint_root_id
    } == std::tuple{
      other.event_tags,
      other.generation_counter,
      other.kin_group_id,
      other.mutation_counter,
      other.program,
      other.root_id,
      other.stint_root_id
    };
  }

  bool operator<(const Genome& other) const {
    // ignore kin_group_epoch_stamps,
    return std::tuple{
      event_tags,
      generation_counter,
      kin_group_id,
      mutation_counter,
      program,
      root_id,
      stint_root_id
    } < std::tuple{
      other.event_tags,
      other.generation_counter,
      other.kin_group_id,
      other.mutation_counter,
      other.program,
      other.root_id,
      other.stint_root_id
    };
  }

  bool operator!=(const Genome& other) const { return !operator==(other); }

  void ElapseGeneration( const size_t rep_lev, const size_t epoch ) {

    generation_counter.ElapseGeneration( rep_lev );
    kin_group_epoch_stamps.ApplyInheritance( rep_lev, epoch );
    kin_group_id.ApplyInheritance( rep_lev );

    if (
      sgpl::tlrand.Get().P( dish2::cfg.MUTATION_RATE()[rep_lev] )
    ) DoMutation();

    // root_id and stint_root_id doesn't change

  }

  void DoMutation() {
    mutation_counter.RecordMutationOccurrence();
    MutateProgram();
    mutation_counter.RecordPointMutation(
      event_tags.ApplyPointMutations( dish2::cfg.POINT_MUTATION_RATE() )
    );
  }

  void MutateProgram() {
    DoProgramSequenceMutation();
    mutation_counter.RecordPointMutation( program.ApplyPointMutations(
      dish2::cfg.POINT_MUTATION_RATE()
    ) );
    // todo, is this convenience worth optimizing out?
    // also potentially would need to fix constructor
    program.RotateGlobalAnchorToFront();
  }

  void DoProgramSequenceMutation() {
    using inst_t = typename program_t::value_type;

    const bool is_severe = sgpl::tlrand.Get().P(
      dish2::cfg.SEVERE_SEQUENCE_MUTATION_RATE()
    );

    // TODO perform the sloppy copy elsewhere for efficiency's sake?
    // do severe sequence mutation
    auto [copy, num_muts] = sgpl::sloppy_copy<inst_t, 0>(
      program,
      dish2::cfg.SEQUENCE_DEFECT_RATE(),
      is_severe ? program.size() : dish2::cfg.MINOR_SEQUENCE_MUTATION_BOUND(),
      dish2::cfg.PROGRAM_MAX_SIZE()
    );
    mutation_counter.RecordInsertionDeletion( num_muts );
    program = std::move( copy );

  }

  void SetupSeededGenotype() {
    kin_group_id = dish2::KinGroupID<Spec>{ std::in_place };
    stint_root_id = dish2::RootID{ std::in_place };
  }

  template <class Archive>
  void serialize( Archive & ar ) { ar(
    CEREAL_NVP( event_tags ),
    CEREAL_NVP( generation_counter ),
    CEREAL_NVP( mutation_counter ),
    CEREAL_NVP( kin_group_id ),
    CEREAL_NVP( program ),
    CEREAL_NVP( root_id ),
    CEREAL_NVP( stint_root_id )
  ); }

};

} // namespace dish2

namespace std {

template <typename Spec>
struct hash<dish2::Genome<Spec>> {

  // hashing bytewise includes neutral padding bits
  // the copy ctor/assignment operator for Program now do memcpy
  // so this should work consistently
  size_t operator()( const dish2::Genome<Spec>& genome ) const {
    const auto& program = genome.program;
    return emp::murmur_hash( std::span<const std::byte>(
      reinterpret_cast<const std::byte*>( program.data() ),
      program.size() * sizeof( program.front() )
    ) );
  }

};

} // namespace std

#endif // #ifndef DISH2_GENOME_GENOME_HPP_INCLUDE
